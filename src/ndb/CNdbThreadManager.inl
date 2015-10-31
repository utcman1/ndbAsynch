template<typename T>
bool CNdbThreadManager<T>::Release()
{
	LOG_NDB_FUNCTION();

	if (!CNdbThreadManager::WaitAllThreadState(ETS_Closed))
		return false;

	if (!CNdbThreadManager::DestroyAllThread())
		return false;

	return true;
}

template<typename T>
bool CNdbThreadManager<T>::CreateAllThread(CNdbClusterConnection& _NdbClusterConnection)
{
	LOG_NDB_FUNCTION();

	m_vecWorker.reserve(m_MaxThreadWorker);

	for (int i = 0; m_MaxThreadWorker > i; ++i)
	{
		CNdbThreadWorker<T>* pWorker = new CNdbThreadWorker<T>(_NdbClusterConnection);
		if (nullptr == pWorker)
		{
			LogNdbCritical << "FAIL : new CNdbThreadWorker()" << endl;
			return false;
		}

		if (!pWorker->Init())
		{
			return false;
		}

		m_vecWorker.push_back(pWorker);
	}

	return true;
}

template<typename T>
bool CNdbThreadManager<T>::DestroyAllThread()
{
	LOG_NDB_FUNCTION();

	for (auto pWorker : m_vecWorker)
	{
		delete pWorker;
	}

	m_vecWorker.clear();

	return true;
}

template<typename T>
bool CNdbThreadManager<T>::CheckAllThreadState(const EThreadState _ThreadState) const
{
	for (auto pWorker : m_vecWorker)
	{
		if (_ThreadState != pWorker->GetState())
			return false;
	}

	return true;
}



template<typename T>
CNdbThreadManager<T>::~CNdbThreadManager()
{
	CNdbThreadManager::Release();
}

template<typename T>
bool CNdbThreadManager<T>::Init(
	CNdbClusterConnection& _NdbClusterConnection, const int _MaxThreadWorker)
{
	LOG_NDB_FUNCTION();

	m_MaxThreadWorker = _MaxThreadWorker;

	if (!CNdbThreadManager::CreateAllThread(_NdbClusterConnection))
		return false;

	if (!CNdbThreadManager::WaitAllThreadState(ETS_Idle))
		return false;

	return true;
}

template<typename T>
bool CNdbThreadManager<T>::WaitAllThreadState(
	const EThreadState _ThreadState, const int _TimeOutSec) const
{
	LOG_NDB_FUNCTION();

	for (int nTry = 0; _TimeOutSec > nTry; ++nTry)
	{
		if (CNdbThreadManager::CheckAllThreadState(_ThreadState))
		{
			LogNdbDebug << "OK" << endl;
			return true;
		}

		NdbSleep_SecSleep(1);
	}

	LogNdbDebug << "FAIL" << endl;
	return false;
}

template<typename T>
bool CNdbThreadManager<T>::TransitIdleToRun()
{
	LOG_NDB_FUNCTION();

	for (auto pWorker : m_vecWorker)
	{
		if (!pWorker->TransitIdleToRun())
		{
			LogNdbDebug << "FAIL" << endl;
			return false;
		}
	}

	LogNdbDebug << "OK" << endl;
	return true;
}
