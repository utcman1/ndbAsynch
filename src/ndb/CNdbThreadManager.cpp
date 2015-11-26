#include <stdafx.hpp>
#include <util/CLog.hpp>
#include <ndb/CNdbThreadState.hpp>
#include <ndb/CNdbThreadWorker.hpp>
#include <ndb/CNdbThreadManager.hpp>



bool CNdbThreadManager::CreateAllThread(
	CNdbClusterConnection& _NdbClusterConnection,
	CNdbThreadContextBuilder& _Builder,
	const int _ThreadWorkerPerThreadManager)
{
	LOG_NDB_FUNCTION();

	m_vecWorker.reserve(_ThreadWorkerPerThreadManager);

	for (int i = 0; _ThreadWorkerPerThreadManager > i; ++i)
	{
		CNdbThreadWorker* pWorker =
			new CNdbThreadWorker();
		assert(nullptr != pWorker);

		if (!pWorker->Init(_NdbClusterConnection, _Builder))
		{
			return false;
		}

		m_vecWorker.push_back(pWorker);
	}

	return true;
}

bool CNdbThreadManager::DestroyAllThread()
{
	LOG_NDB_FUNCTION();

	for (auto pWorker : m_vecWorker)
	{
		delete pWorker;
	}

	m_vecWorker.clear();

	return true;
}

void CNdbThreadManager::Release()
{
	LOG_NDB_FUNCTION();

	CNdbThreadManager::WaitAllThreadState(ETS_Closed);
	CNdbThreadManager::DestroyAllThread();
}

bool CNdbThreadManager::CheckAllThreadState(
	const EThreadState _ThreadState) const
{
	for (auto pWorker : m_vecWorker)
	{
		if (_ThreadState != pWorker->GetState())
			return false;
	}

	return true;
}



CNdbThreadManager::~CNdbThreadManager()
{
	CNdbThreadManager::Release();
}

bool CNdbThreadManager::Init(CNdbClusterConnection& _NdbClusterConnection,
	CNdbThreadContextBuilder& _Builder,
	const int _ThreadWorkerPerThreadManager)
{
	LOG_NDB_FUNCTION();

	if (!CNdbThreadManager::CreateAllThread(
		_NdbClusterConnection, _Builder, _ThreadWorkerPerThreadManager))
		return false;

	if (!CNdbThreadManager::WaitAllThreadState(ETS_Idle))
		return false;

	return true;
}

bool CNdbThreadManager::WaitAllThreadState(
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

bool CNdbThreadManager::TransitIdleToRun()
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
