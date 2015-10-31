#include <stdafx.hpp>
#include <const.hpp>
#include <util/CLog.hpp>
#include <ndb/CNdbInit.hpp>
#include <ndb/CNdbClusterConnection.hpp>
#include <ndb/CNdb.hpp>
#include <ndb/CNdbRecordSpec.hpp>
#include <ndb/CNdbRecordSpec.inl>
#include <ndb/CNdbThreadState.hpp>
#include <ndb/CNdbThreadState.inl>
#include <ndb/CNdbThreadWorker.hpp>
#include <ndb/CNdbThreadWorker.inl>
#include <ndb/CNdbThreadManager.hpp>
#include <ndb/CNdbThreadManager.inl>
#include <user/CUserRecord.hpp>
#include <user/CUserThreadState.hpp>



int main()
{
	const CNdbInit NdbInit;
#ifdef _WIN32
	CNdbClusterConnection NdbClusterConnection(NdbInit, 255, "192.168.137.101:1186");
#else
	CNdbClusterConnection NdbClusterConnection(NdbInit, 0);
#endif
	if (!NdbClusterConnection.Init())
		return -1;

	CNdbThreadManager<CUserThreadState> NdbThreadManager;
	if (!NdbThreadManager.Init(NdbClusterConnection, MaxThreadWorker))
		return -1;

	NdbThreadManager.TransitIdleToRun();
	NdbThreadManager.WaitAllThreadState(ETS_Closed);

	return 0;
}
