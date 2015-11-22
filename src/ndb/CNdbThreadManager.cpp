#include <stdafx.hpp>
#include <util/CLog.hpp>
#include <ndb/CNdbThreadState.hpp>
#include <ndb/CNdbThreadContext.hpp>
#include <ndb/CNdbThreadWorker.hpp>
#include <ndb/CNdbThreadManager.hpp>



bool CNdbThreadManager::Release()
{
	LOG_NDB_FUNCTION();

	if (!CNdbThreadManager::WaitAllThreadState(ETS_Closed))
		return false;

	if (!CNdbThreadManager::DestroyAllThread())
		return false;

	return true;
}

bool CNdbThreadManager::CreateAllThread(
	CNdbClusterConnection& _NdbClusterConnection,
	CNdbThreadContextImplBuilder& _ImplBuilder, const int _MaxThreadWorker)
{
	LOG_NDB_FUNCTION();

	m_vecWorker.reserve(_MaxThreadWorker);

	for (int i = 0; _MaxThreadWorker > i; ++i)
	{
		CNdbThreadWorker* pWorker =
			new CNdbThreadWorker(_NdbClusterConnection);
		assert(nullptr != pWorker);

		if (!pWorker->Init(_ImplBuilder))
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

bool CNdbThreadManager::CheckAllThreadState(const EThreadState _ThreadState) const
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
	CNdbThreadContextImplBuilder& _ImplBuilder, const int _MaxThreadWorker)
{
	LOG_NDB_FUNCTION();

	if (!CNdbThreadManager::CreateAllThread(
		_NdbClusterConnection, _ImplBuilder, _MaxThreadWorker))
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
