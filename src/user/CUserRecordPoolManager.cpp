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

	m_pRecordPoolVector.reserve(RecordPoolPerThreadWorker);

	for (int i = 0; RecordPoolPerThreadWorker > i; ++i)
	{
		CUserRecordPool* pRecordPool =
			new CUserRecordPool(_NdbClusterConnection);
		if (!pRecordPool->Init())
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

int CUserRecordPoolManager::PrepareAndSendTran(const int _IdxBegin)
{
	LOG_USER_FUNCTION();

	int Idx = _IdxBegin;
	for (auto pRecordPool : m_pRecordPoolVector)
	{
		for(; nullptr != USER_CALL_FUNCTION(
			pRecordPool->EnqueTran(Idx)); ++Idx)
		{
		}

		USER_CALL_FUNCTION(pRecordPool->sendPreparedTransactions());
	}

	return Idx - _IdxBegin;
}

int CUserRecordPoolManager::CompleteTran()
{
	LOG_USER_FUNCTION();

	int CompleteTranCount = 0;
	for (auto pRecordPool : m_pRecordPoolVector)
	{
		int waitTime = 3000;

		CompleteTranCount +=
			USER_CALL_FUNCTION(pRecordPool->pollNdb(waitTime, 0));
	}

	return CompleteTranCount;
}
