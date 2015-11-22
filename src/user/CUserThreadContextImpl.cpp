#include <stdafx.hpp>
#include <util/CLog.hpp>
#include <ndb/CNdbThreadContextImpl.hpp>
#include <ndb/CNdbThreadContext.hpp>
#include <ndb/CNdb.hpp>
#include <ndb/CNdbRecordSpec.hpp>
#include <user/CUserRecord.hpp>
#include <user/CUserRecordPool.hpp>
#include <user/CUserThreadContextImpl.hpp>



void CUserThreadContextImpl::OnCreate()
{
	LOG_USER_FUNCTION();

	CUserRecordPool::InitOnCreate(*(m_pContext->GetNdb()));
}

void CUserThreadContextImpl::OnIdle()
{
	//LOG_USER_FUNCTION();
}

void CUserThreadContextImpl::OnRun()
{
	LOG_USER_FUNCTION();

	static int TotalCompleteTranCount = 0;
	static int PrintTranStep = 100000;
	static int NextPrintTranCount = PrintTranStep;

	int EnqueueTranCount = CUserRecordPool::EnqueLoop(*(m_pContext->GetNdb()));
	int CompleteTranCount = USER_CALL_FUNCTION(m_pContext->GetNdb()->sendPollNdb(3000));

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

void CUserThreadContextImpl::OnClosing()
{
	LOG_USER_FUNCTION();

	//CNdbThreadState<CUserThreadState>::TransitClosingToDestroy();
}

void CUserThreadContextImpl::OnDestroy()
{
	LOG_USER_FUNCTION();
}
