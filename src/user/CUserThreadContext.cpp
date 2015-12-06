#include <stdafx.hpp>
#include <util/CLog.hpp>
#include <ndb/CNdbClusterConnection.hpp>
#include <ndb/CNdbThreadState.hpp>
#include <ndb/CNdbThreadContext.hpp>
#include <user/CUserRecordPoolManager.hpp>
#include <user/CUserThreadContext.hpp>



void CUserThreadContext::Release()
{
	LOG_USER_FUNCTION();
}



CUserThreadContext::CUserThreadContext(CNdbThreadState& _NdbThreadState)
	: m_NdbThreadState(_NdbThreadState)
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

	if (!m_RecordPoolManager.Init(_NdbClusterConnection))
		return false;

	return true;
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

	static int TotalPreparedTranCount = 0;
	static int TotalCompleteTranCount = 0;
	static int PrintTranStep = 100000;
	static int NextPrintTranCount = PrintTranStep;

	TotalPreparedTranCount += USER_CALL_FUNCTION(
		m_RecordPoolManager.PrepareAndSendTran(TotalPreparedTranCount));
	TotalCompleteTranCount += USER_CALL_FUNCTION(
		m_RecordPoolManager.CompleteTran());

	if (TotalCompleteTranCount >= NextPrintTranCount)
	{
		NextPrintTranCount += PrintTranStep;
		LogUserWarning << "TotalCompleteTranCount : "
			<< TotalCompleteTranCount << endl;
	}

	//if (1000000 < TotalCompleteTranCount)
	//{
	//	LogUserWarning << "TransitRunToClosing()" << endl;
	//	m_NdbThreadState.TransitRunToClosing();
	//}
}

void CUserThreadContext::OnClosing()
{
	LOG_USER_FUNCTION();

	m_NdbThreadState.TransitClosingToDestroy();
}

void CUserThreadContext::OnDestroy()
{
	LOG_USER_FUNCTION();
}
