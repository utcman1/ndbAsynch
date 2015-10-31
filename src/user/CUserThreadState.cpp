#include <stdafx.hpp>
#include <const.hpp>
#include <util/CLog.hpp>
#include <ndb/CNdb.hpp>
#include <ndb/CNdbRecordSpec.hpp>
#include <ndb/CNdbRecordSpec.inl>
#include <user/CUserRecord.hpp>
#include <user/CUserThreadState.hpp>



void CUserThreadState::UserAsynchCallback(int _Result,
	NdbTransaction* _pTran, void* _pRecord)
{
	if (0 != _Result)
	{
		LogUserError << _pTran->getNdbError() << endl;
	}

	CTestRecord* pRecord = static_cast<CTestRecord*>(_pRecord);
	pRecord->pUserThreadState->DequeTran(pRecord);
}



bool CUserThreadState::InitRecordSpec(CNdb& _Ndb)
{
	LOG_USER_FUNCTION();

	if (0 != USER_CALL_FUNCTION(_Ndb.setDatabaseName("d_ndb_test")))
	{
		LogUserCritical << _Ndb.getNdbError() << endl;
		return false;
	}

	m_pKeyRecordSpec = new CNdbRecordSpec<CTestKey>(_Ndb);
	if (nullptr == m_pKeyRecordSpec)
	{
		LogUserCritical << "FAIL : new CNdbRecordSpec<CTestKey>()" << endl;
		return false;
	}

	m_pDataRecordSpec = new CNdbRecordSpec<CTestData>(_Ndb);
	if (nullptr == m_pDataRecordSpec)
	{
		LogUserCritical << "FAIL : new CNdbRecordSpec<CTestData>()" << endl;
		return false;
	}

	if (!m_pKeyRecordSpec->Init())
	{
		LogUserCritical << _Ndb.getNdbError() << endl;
		return false;
	}

	if (!m_pDataRecordSpec->Init())
	{
		LogUserCritical << _Ndb.getNdbError() << endl;
		return false;
	}

	return true;
}

bool CUserThreadState::InitRecordPool()
{
	LOG_USER_FUNCTION();

	m_vecRecordPool.resize(256);

	for (size_t i = 0; m_vecRecordPool.capacity() > i; ++i)
	{
		m_vecRecordPool[i].pUserThreadState = this;
		m_stackFreeRecordPool.push(&m_vecRecordPool[i]);
	}

	return true;
}

bool CUserThreadState::InitOnCreate(CNdb& _Ndb)
{
	LOG_USER_FUNCTION();

	if (!InitRecordSpec(_Ndb))
		return false;

	if (!InitRecordPool())
		return false;

	return true;
}

CTestRecord* CUserThreadState::AllocRecord()
{
	LOG_USER_FUNCTION();

	if (m_stackFreeRecordPool.empty())
		return nullptr;

	auto ret = m_stackFreeRecordPool.top();
	m_stackFreeRecordPool.pop();
	return ret;
}

bool CUserThreadState::FreeRecord(CTestRecord* _pRecord)
{
	LOG_USER_FUNCTION();

	_pRecord->pTran = nullptr;
	m_stackFreeRecordPool.push(_pRecord);
	return true;
}

CTestRecord* CUserThreadState::EnqueTran(CNdb& _Ndb, const int _Num)
{
	LOG_USER_FUNCTION();

	CTestRecord* pRecord = CUserThreadState::AllocRecord();
	if (nullptr == pRecord)
	{
		// 실패하는 경우의 로그는 이미 있다.
		return nullptr;
	}

	NdbTransaction* pTran = USER_CALL_FUNCTION(_Ndb.startTransaction());
	if (nullptr == pTran)
	{
		LogUserError << _Ndb.getNdbError() << endl;
		CUserThreadState::FreeRecord(pRecord);
		return nullptr;
	}

	// 데이터 설정.
	pRecord->pTran = pTran;
	pRecord->Key.a = (_Num%100000) + 1;
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
		m_pKeyRecordSpec->GetNdbRecordPtr(),
		reinterpret_cast<const char*>(&(pRecord->Key)),
		m_pDataRecordSpec->GetNdbRecordPtr(),
		reinterpret_cast<char*>(&(pRecord->Data))));

	if (nullptr == pOper)
	{
		LogUserError << pTran->getNdbError() << endl;
		FreeRecord(pRecord);
		_Ndb.closeTransaction(pTran);
		return nullptr;
	}

	USER_CALL_FUNCTION(pTran->executeAsynchPrepare(NdbTransaction::Commit,
		&CUserThreadState::UserAsynchCallback, pRecord));
	return pRecord;
}

bool CUserThreadState::DequeTran(CTestRecord* _pRecord)
{
	LOG_USER_FUNCTION();

	USER_CALL_FUNCTION(_pRecord->pTran->close());

	if (!CUserThreadState::FreeRecord(_pRecord))
		return false;

	return true;
}

int CUserThreadState::EnqueLoop(CNdb& _Ndb)
{
	LOG_USER_FUNCTION();
	static int idx = 1;

	int BeginIdx = idx;

	for (; nullptr != EnqueTran(_Ndb, idx); ++idx)
	{
	}

	return (idx - BeginIdx);
}
