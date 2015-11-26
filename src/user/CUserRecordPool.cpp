#include <stdafx.hpp>
#include <util/CLog.hpp>
#include <ndb/CNdb.hpp>
#include <ndb/CNdbRecordSpec.hpp>
#include <ndb/CNdbRecordSpec.inl>
#include <user/CUserRecord.hpp>
#include <user/CUserRecordPool.hpp>



void CUserRecordPool::UserAsynchCallback(int _Result,
	NdbTransaction* _pTran, void* _pRecord)
{
	if (0 != _Result)
	{
		LogUserError << _pTran->getNdbError() << endl;
	}

	CTestRecord* pRecord = static_cast<CTestRecord*>(_pRecord);
	pRecord->pUserRecordPool->DequeTran(pRecord);
}



bool CUserRecordPool::InitNdbDatabse()
{
	LOG_USER_FUNCTION();

	if (0 != USER_CALL_FUNCTION(CNdb::setDatabaseName("d_ndb_test")))
	{
		LogUserCritical << CNdb::getNdbError() << endl;
		return false;
	}

	return true;
}

bool CUserRecordPool::InitRecordSpec()
{
	if (!m_KeyRecordSpec.Init())
	{
		LogUserCritical << CNdb::getNdbError() << endl;
		return false;
	}

	if (!m_DataRecordSpec.Init())
	{
		LogUserCritical << CNdb::getNdbError() << endl;
		return false;
	}

	return true;
}

bool CUserRecordPool::InitRecordPool(const int _RecordPerRecordPool)
{
	LOG_USER_FUNCTION();

	m_vecRecordPool.resize(_RecordPerRecordPool);

	for (size_t i = 0; m_vecRecordPool.capacity() > i; ++i)
	{
		m_vecRecordPool[i].pUserRecordPool = this;
		m_stackFreeRecordPool.push(&m_vecRecordPool[i]);
	}

	return true;
}

CTestRecord* CUserRecordPool::AllocRecord()
{
	LOG_USER_FUNCTION();

	if (m_stackFreeRecordPool.empty())
		return nullptr;

	auto ret = m_stackFreeRecordPool.top();
	m_stackFreeRecordPool.pop();
	return ret;
}

bool CUserRecordPool::FreeRecord(CTestRecord* _pRecord)
{
	LOG_USER_FUNCTION();

	_pRecord->pTran = nullptr;
	m_stackFreeRecordPool.push(_pRecord);
	return true;
}

CTestRecord* CUserRecordPool::EnqueTran(const int _Idx)
{
	LOG_USER_FUNCTION();

	CTestRecord* pRecord = CUserRecordPool::AllocRecord();
	if (nullptr == pRecord)
	{
		// 실패하는 경우의 로그는 이미 있다.
		return nullptr;
	}

	NdbTransaction* pTran = USER_CALL_FUNCTION(CNdb::startTransaction());
	if (nullptr == pTran)
	{
		LogUserError << CNdb::getNdbError() << endl;
		CUserRecordPool::FreeRecord(pRecord);
		return nullptr;
	}

	// 데이터 설정.
	pRecord->pTran = pTran;
	pRecord->Key.a = (_Idx % MaxDataCount) + 1;
	//pRecord->Data.a = _Num;
	//pRecord->Data.b = _Num;
	pRecord->Data.a = 0;
	pRecord->Data.b = 0;

	// NdbOperation
	//const NdbOperation* pOper = USER_CALL_FUNCTION(pTran->writeTuple(
	//	m_pKeyRecordSpec->GetNdbRecordPtr(),
	//	reinterpret_cast<const char*>(&(pRecord->Key)),
	//	m_pDataRecordSpec->GetNdbRecordPtr(),
	//	reinterpret_cast<const char*>(&(pRecord->Data))));
	const NdbOperation* pOper = USER_CALL_FUNCTION(pTran->readTuple(
		m_KeyRecordSpec.GetNdbRecordPtr(),
		reinterpret_cast<const char*>(&(pRecord->Key)),
		m_DataRecordSpec.GetNdbRecordPtr(),
		reinterpret_cast<char*>(&(pRecord->Data))));

	if (nullptr == pOper)
	{
		LogUserError << pTran->getNdbError() << endl;
		FreeRecord(pRecord);
		CNdb::closeTransaction(pTran);
		return nullptr;
	}

	USER_CALL_FUNCTION(pTran->executeAsynchPrepare(NdbTransaction::Commit,
		&CUserRecordPool::UserAsynchCallback, pRecord));
	return pRecord;
}

bool CUserRecordPool::DequeTran(CTestRecord* _pRecord)
{
	LOG_USER_FUNCTION();

	USER_CALL_FUNCTION(_pRecord->pTran->close());

	if (!CUserRecordPool::FreeRecord(_pRecord))
		return false;

	return true;
}



CUserRecordPool::CUserRecordPool(CNdbClusterConnection& _NdbClusterConnection)
	: CNdb(_NdbClusterConnection),
	m_KeyRecordSpec(*this), m_DataRecordSpec(*this)
{
	LOG_USER_FUNCTION();
}

bool CUserRecordPool::Init(const int _RecordPerRecordPool)
{
	LOG_USER_FUNCTION();

	if (!CNdb::Init(_RecordPerRecordPool))
		return false;

	if (!CUserRecordPool::InitNdbDatabse())
		return false;

	if (!CUserRecordPool::InitRecordSpec())
		return false;

	if (!CUserRecordPool::InitRecordPool(_RecordPerRecordPool))
		return false;

	return true;
}

int CUserRecordPool::PrepareTransactions()
{
	LOG_USER_FUNCTION();
	static int idx = 1;

	int BeginIdx = idx;

	for (; nullptr != EnqueTran(idx); ++idx)
	{
	}

	return (idx - BeginIdx);
}
