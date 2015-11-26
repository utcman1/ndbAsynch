#include <stdafx.hpp>
#include <util/CLog.hpp>
#include <ndb/CNdbClusterConnection.hpp>
#include <ndb/CNdbThreadContext.hpp>
#include <user/CUserThreadContext.hpp>

// for CUserRecordPool
#include <ndb/CNdb.hpp>
#include <ndb/CNdbRecordSpec.hpp>
#include <user/CUserRecord.hpp>
#include <user/CUserRecordPool.hpp>



bool CUserThreadContext::CreateRecordPoolVector(
	CNdbClusterConnection& _NdbClusterConnection)
{
	LOG_USER_FUNCTION();

	m_pRecordPoolVector.reserve(MaxRecordPoolPerThread);

	for (int i = 0; MaxRecordPoolPerThread > i; ++i)
	{
		CUserRecordPool* pRecordPool = new CUserRecordPool(_NdbClusterConnection);
		if (!pRecordPool->Init())
			return false;

		m_pRecordPoolVector.push_back(pRecordPool);
	}

	return true;
}

void CUserThreadContext::DestroyRecordPoolVector()
{
	LOG_USER_FUNCTION();

	for (CUserRecordPool*& pRecordPool : m_pRecordPoolVector)
	{
		delete pRecordPool;
		pRecordPool = nullptr;
	}

	m_pRecordPoolVector.clear();
}

void CUserThreadContext::Release()
{
	LOG_USER_FUNCTION();

	CUserThreadContext::DestroyRecordPoolVector();
}



CUserThreadContext::CUserThreadContext()
{
	LOG_USER_FUNCTION();
}

CUserThreadContext::~CUserThreadContext()
{
	LOG_USER_FUNCTION();

	CUserThreadContext::Release();
}

bool CUserThreadContext::Init(CNdbClusterConnection& _NdbClusterConnection)
{
	LOG_USER_FUNCTION();

	if (!CreateRecordPoolVector(_NdbClusterConnection))
		return false;

	return true;
}

CUserRecordPool* CUserThreadContext::GetRecordPool()
{
	return m_pRecordPoolVector[0];
}

void CUserThreadContext::OnCreate()
{
	LOG_USER_FUNCTION();
}

void CUserThreadContext::OnIdle()
{
	LOG_USER_FUNCTION();
}

void CUserThreadContext::OnRun()
{
	LOG_USER_FUNCTION();

	static int TotalCompleteTranCount = 0;
	static int PrintTranStep = 100000;
	static int NextPrintTranCount = PrintTranStep;

	int EnqueueTranCount = CUserThreadContext::GetRecordPool()->EnqueLoop();
	int CompleteTranCount = USER_CALL_FUNCTION(
		CUserThreadContext::GetRecordPool()->sendPollNdb(3000));

	//LogUserWarning << "Enqueue [" << EnqueueTranCount << "] / Complete [" << CompleteTranCount << "] Transactions" << endl;

	TotalCompleteTranCount += CompleteTranCount;
	if (TotalCompleteTranCount >= NextPrintTranCount)
	{
		NextPrintTranCount += PrintTranStep;
		LogUserWarning << "TotalCompleteTranCount : " << TotalCompleteTranCount << endl;
	}

	//if (100000 < TotalCompleteTranCount)
	//{
	//	LogUserWarning << "TransitRunToClosing()" << endl;
	//	CNdbThreadState<CUserThreadState>::TransitRunToClosing();
	//}
}

void CUserThreadContext::OnClosing()
{
	LOG_USER_FUNCTION();

	//CNdbThreadState<CUserThreadState>::TransitClosingToDestroy();
}

void CUserThreadContext::OnDestroy()
{
	LOG_USER_FUNCTION();
}



CNdbThreadContext* CUserThreadContextBuilder::Create()
{
	LOG_USER_FUNCTION();

	return new CUserThreadContext();
}
