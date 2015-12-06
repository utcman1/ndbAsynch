#include <stdafx.hpp>
#include <util/CLog.hpp>
#include <ndb/CNdbClusterConnection.hpp>
#include <user/CUserRecordPoolManager.hpp>

// for CUserRecordPool
#include <ndb/CNdb.hpp>
#include <ndb/CNdbRecordSpec.hpp>
#include <ndb/CNdbRecordSpec.inl>
#include <user/CUserRecord.hpp>
#include <user/CUserRecordPool.hpp>



bool CUserRecordPoolManager::CreateRecordPoolVector(
	CNdbClusterConnection& _NdbClusterConnection)
{
	LOG_USER_FUNCTION();

	m_pRecordPoolVector.reserve(NdbPartitionCount);

	for (int PartId = 0; NdbPartitionCount > PartId; ++PartId)
	{
		CUserRecordPool* pRecordPool =
			new CUserRecordPool(_NdbClusterConnection);
		if (!pRecordPool->Init(PartId))
			return false;

		m_pRecordPoolVector.push_back(pRecordPool);
	}

	return true;
}

void CUserRecordPoolManager::DestroyRecordPoolVector()
{
	LOG_USER_FUNCTION();

	for (CUserRecordPool*& pRecordPool : m_pRecordPoolVector)
	{
		delete pRecordPool;
		pRecordPool = nullptr;
	}

	m_pRecordPoolVector.clear();
}

void CUserRecordPoolManager::Release()
{
	LOG_USER_FUNCTION();

	CUserRecordPoolManager::DestroyRecordPoolVector();
}



CUserRecordPoolManager::~CUserRecordPoolManager()
{
	LOG_USER_FUNCTION();

	CUserRecordPoolManager::Release();
}

bool CUserRecordPoolManager::Init(CNdbClusterConnection& _NdbClusterConnection)
{
	LOG_USER_FUNCTION();

	if (!CUserRecordPoolManager::
		CreateRecordPoolVector(_NdbClusterConnection))
		return false;

	return true;
}

int CUserRecordPoolManager::CompleteTran()
{
	LOG_USER_FUNCTION();

	int CompleteTranCount = 0;
	bool firstRecordPool = true;
	for (auto pRecordPool : m_pRecordPoolVector)
	{
		// waitTime은 첫번째 RecordPool에서만 적용한다.
		int waitTime = firstRecordPool ? 1 : 0;
		firstRecordPool = false;

		CompleteTranCount +=
			USER_CALL_FUNCTION(pRecordPool->pollNdb(waitTime));
	}

	return CompleteTranCount;
}

int CUserRecordPoolManager::PrepareTran(const int _IdxBegin)
{
	LOG_USER_FUNCTION();

	int idx = _IdxBegin;
	std::size_t PartitionId = -1;

	do
	{
		++idx;
		PartitionId = m_pRecordPoolVector[0]->GetPartitionId(idx);
		assert(PartitionId < m_pRecordPoolVector.size());
	} while (nullptr != USER_CALL_FUNCTION(
		m_pRecordPoolVector[PartitionId]->EnqueTran(idx)));

	return (idx - _IdxBegin - 1);
}

void CUserRecordPoolManager::SendTran()
{
	LOG_USER_FUNCTION();

	for (auto pRecordPool : m_pRecordPoolVector)
	{
		USER_CALL_FUNCTION(pRecordPool->sendPreparedTransactions());
	}
}
