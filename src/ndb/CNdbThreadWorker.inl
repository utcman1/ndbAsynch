template<typename T>
void* CNdbThreadWorker<T>::StaticMain(void* _param)
{
	LOG_NDB_FUNCTION();

	CNdbThreadWorker<T>* pNdbThreadWorker = static_cast<CNdbThreadWorker<T>*>(_param);
	assert(ETS_Create == pNdbThreadWorker->GetState());
	return pNdbThreadWorker->WorkerMain();
}



template<typename T>
bool CNdbThreadWorker<T>::Release()
{
	LOG_NDB_FUNCTION();

	return CNdbThreadWorker<T>::DestroyThread();
}

template<typename T>
bool CNdbThreadWorker<T>::CreateThread()
{
	LOG_NDB_FUNCTION();

	assert(ETS_Invalid == CNdbThreadState<T>::GetState());
	assert(nullptr == m_pThread);

	CNdbThreadState<T>::TransitInvalidToCreate();

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

template<typename T>
bool CNdbThreadWorker<T>::DestroyThread()
{
	LOG_NDB_FUNCTION();

	assert(ETS_Closed == CNdbThreadState<T>::GetState());

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



template<typename T>
CNdbThreadWorker<T>::CNdbThreadWorker(CNdbClusterConnection& _NdbClusterConnection)
	: CNdbThreadState<T>(_NdbClusterConnection)
{
	LOG_NDB_FUNCTION();
}

template<typename T>
CNdbThreadWorker<T>::~CNdbThreadWorker()
{
	LOG_NDB_FUNCTION();

	CNdbThreadWorker<T>::Release();
}

template<typename T>
bool CNdbThreadWorker<T>::Init()
{
	LOG_NDB_FUNCTION();

	assert(ETS_Invalid == CNdbThreadState<T>::GetState());

	if (!CNdbThreadState<T>::Init())
		return false;

	if (!CNdbThreadWorker<T>::CreateThread())
		return false;

	return true;
}

template<typename T>
void* CNdbThreadWorker<T>::WorkerMain()
{
	LOG_NDB_FUNCTION();

	bool bContinue = true;

	while(bContinue)
	{
		switch (CNdbThreadState<T>::GetState())
		{
		case ETS_Create:
			CNdbThreadState<T>::OnCreate();
			CNdbThreadState<T>::TransitCreateToIdle();
			break;

		case ETS_Idle:
			CNdbThreadState<T>::OnIdle();
			break;

		case ETS_Run:
			CNdbThreadState<T>::OnRun();
			break;

		case ETS_Closing:
			CNdbThreadState<T>::OnClosing();
			break;

		case ETS_Destroy:
			CNdbThreadState<T>::OnDestroy();
			CNdbThreadState<T>::TransitDestroyToClosed();
			bContinue = false;
			break;

		default:
			break;
		}
	}

	return nullptr;
}
