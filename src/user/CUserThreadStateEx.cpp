#include <stdafx.hpp>
#include <util/CLog.hpp>
#include <ndb/CNdb.hpp>
#include <ndb/CNdbRecordSpec.hpp>
//#include <ndb/CNdbRecordSpec.inl>
#include <ndb/CNdbThreadState.hpp>
//#include <ndb/CNdbThreadState.inl>
#include <user/CUserRecord.hpp>
#include <user/CUserThreadState.hpp>
#include <user/CUserThreadStateEx.hpp>



template<>
void CNdbThreadState<CUserThreadState>::OnCreate()
{
	LOG_USER_FUNCTION();

	CUserThreadState::InitOnCreate(*this);
}

template<>
void CNdbThreadState<CUserThreadState>::OnIdle()
{
	//LOG_USER_FUNCTION();
}

template<>
void CNdbThreadState<CUserThreadState>::OnRun()
{
	LOG_USER_FUNCTION();

	static int TotalCompleteTranCount = 0;
	static int PrintTranStep = 100000;
	static int NextPrintTranCount = PrintTranStep;

	int EnqueueTranCount = CUserThreadState::EnqueLoop(*this);
	int CompleteTranCount = USER_CALL_FUNCTION(CNdb::sendPollNdb(3000));

	//LogUserWarning << "Enqueue [" << EnqueueTranCount << "] / Complete [" << CompleteTranCount << "] Transactions" << endl;

	TotalCompleteTranCount += CompleteTranCount;
	if (TotalCompleteTranCount >= NextPrintTranCount)
	{
		NextPrintTranCount += PrintTranStep;
		LogUserWarning << "TotalCompleteTranCount : " << TotalCompleteTranCount << endl;
	}

	if (100000 < TotalCompleteTranCount)
	{
		LogUserWarning << "TransitRunToClosing()" << endl;
		CNdbThreadState<CUserThreadState>::TransitRunToClosing();
	}
}

template<>
void CNdbThreadState<CUserThreadState>::OnClosing()
{
	LOG_USER_FUNCTION();

	CNdbThreadState<CUserThreadState>::TransitClosingToDestroy();
}

template<>
void CNdbThreadState<CUserThreadState>::OnDestroy()
{
	LOG_USER_FUNCTION();
}
