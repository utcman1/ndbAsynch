#include <stdafx.hpp>
#include <util/CLog.hpp>
#include <ndb/CNdb.hpp>
#include <ndb/CNdbThreadState.hpp>
#include <ndb/CNdbThreadWorker.hpp>

#include <ndb/CNdbThreadContext.hpp>



void* CNdbThreadWorker::StaticMain(void* _param)
{
	LOG_NDB_FUNCTION();

	CNdbThreadWorker* pNdbThreadWorker =
		static_cast<CNdbThreadWorker*>(_param);
	assert(ETS_Create == pNdbThreadWorker->GetState());
	return pNdbThreadWorker->WorkerMain();
}



bool CNdbThreadWorker::CreateContext(
	CNdbClusterConnection& _NdbClusterConnection,
	CNdbThreadContextBuilder& _Builder)
{
	LOG_NDB_FUNCTION();

	assert(nullptr == m_pContext);
	m_pContext = _Builder.Create(*this);

	if (nullptr == m_pContext)
		return false;

	return m_pContext->Init(_NdbClusterConnection);
}

void CNdbThreadWorker::DestroyContext()
{
	LOG_NDB_FUNCTION();

	delete m_pContext;
	m_pContext = nullptr;
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

void CNdbThreadWorker::DestroyThread()
{
	LOG_NDB_FUNCTION();

	assert(ETS_Closed == CNdbThreadState::GetState());

	if (nullptr == m_pThread)
		return;

	void* threadStatus = nullptr;
	if (0 != NDB_CALL_FUNCTION(NdbThread_WaitFor(m_pThread, &threadStatus)))
	{
		LogNdbCritical << "FAIL : NdbThread_WaitFor()" << endl;
		return;
	}

	NDB_CALL_FUNCTION(NdbThread_Destroy(&m_pThread));
}

void CNdbThreadWorker::Release()
{
	LOG_NDB_FUNCTION();

	CNdbThreadWorker::DestroyThread();
	CNdbThreadWorker::DestroyContext();
}



CNdbThreadWorker::CNdbThreadWorker()
{
	LOG_NDB_FUNCTION();
}

CNdbThreadWorker::~CNdbThreadWorker()
{
	LOG_NDB_FUNCTION();

	CNdbThreadWorker::Release();
}

bool CNdbThreadWorker::Init(CNdbClusterConnection& _NdbClusterConnection,
	CNdbThreadContextBuilder& _Builder)
{
	LOG_NDB_FUNCTION();

	assert(ETS_Invalid == CNdbThreadState::GetState());

	if (!CNdbThreadWorker::CreateContext(_NdbClusterConnection, _Builder))
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
			m_pContext->OnCreate();
			CNdbThreadState::TransitCreateToIdle();
			break;

		case ETS_Idle:
			m_pContext->OnIdle();
			break;

		case ETS_Run:
			m_pContext->OnRun();
			break;

		case ETS_Closing:
			m_pContext->OnClosing();
			break;

		case ETS_Destroy:
			m_pContext->OnDestroy();
			CNdbThreadState::TransitDestroyToClosed();
			bContinue = false;
			break;

		default:
			break;
		}
	}

	return nullptr;
}
