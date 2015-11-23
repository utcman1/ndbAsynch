#include <stdafx.hpp>
#include <util/CLog.hpp>
#include <ndb/CNdb.hpp>
#include <ndb/CNdbThreadState.hpp>
#include <ndb/CNdbThreadContext.hpp>
#include <ndb/CNdbThreadWorker.hpp>



void* CNdbThreadWorker::StaticMain(void* _param)
{
	LOG_NDB_FUNCTION();

	CNdbThreadWorker* pNdbThreadWorker =
		static_cast<CNdbThreadWorker*>(_param);
	assert(ETS_Create == pNdbThreadWorker->GetState());
	return pNdbThreadWorker->WorkerMain();
}



bool CNdbThreadWorker::CreateThread()
{
	LOG_NDB_FUNCTION();

	assert(ETS_Invalid == CNdbThreadState::GetState());
	assert(nullptr == m_pThread);

	CNdbThreadState::TransitInvalidToCreate();

	m_pThread = NDB_CALL_FUNCTION(NdbThread_Create(
		StaticMain, reinterpret_cast<NDB_THREAD_ARG*>(this),
		32768, "ThreadWorker", NDB_THREAD_PRIO_LOW));
	if (nullptr == m_pThread)
	{
		LogNdbCritical << "FAIL : NdbThread_Create()" << endl;
		return false;
	}

	return true;
}

bool CNdbThreadWorker::DestroyThread()
{
	LOG_NDB_FUNCTION();

	assert(ETS_Closed == CNdbThreadState::GetState());

	if (nullptr == m_pThread)
		return true;

	void* threadStatus = nullptr;
	if (0 != NDB_CALL_FUNCTION(NdbThread_WaitFor(m_pThread, &threadStatus)))
	{
		LogNdbCritical << "FAIL : NdbThread_WaitFor()" << endl;
		return false;
	}

	NDB_CALL_FUNCTION(NdbThread_Destroy(&m_pThread));

	return true;
}

void CNdbThreadWorker::Release()
{
	LOG_NDB_FUNCTION();

	CNdbThreadWorker::DestroyThread();
}



CNdbThreadWorker::CNdbThreadWorker(
	CNdbClusterConnection& _NdbClusterConnection)
	: CNdbThreadContext(_NdbClusterConnection)
{
	LOG_NDB_FUNCTION();
}

CNdbThreadWorker::~CNdbThreadWorker()
{
	LOG_NDB_FUNCTION();

	CNdbThreadWorker::Release();
}

bool CNdbThreadWorker::Init(CNdbThreadContextImplBuilder& _ImplBuilder)
{
	LOG_NDB_FUNCTION();

	assert(ETS_Invalid == CNdbThreadState::GetState());

	if (!CNdbThreadContext::Init(_ImplBuilder))
		return false;

	if (!CNdbThreadWorker::CreateThread())
		return false;

	return true;
}

void* CNdbThreadWorker::WorkerMain()
{
	LOG_NDB_FUNCTION();

	bool bContinue = true;

	while(bContinue)
	{
		switch (CNdbThreadState::GetState())
		{
		case ETS_Create:
			CNdbThreadContext::OnCreate();
			CNdbThreadState::TransitCreateToIdle();
			break;

		case ETS_Idle:
			CNdbThreadContext::OnIdle();
			break;

		case ETS_Run:
			CNdbThreadContext::OnRun();
			break;

		case ETS_Closing:
			CNdbThreadContext::OnClosing();
			break;

		case ETS_Destroy:
			CNdbThreadContext::OnDestroy();
			CNdbThreadState::TransitDestroyToClosed();
			bContinue = false;
			break;

		default:
			break;
		}
	}

	return nullptr;
}
