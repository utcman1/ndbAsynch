#include <stdafx.hpp>
#include <ndb/CNdbInit.hpp>
#include <ndb/CNdbClusterConnection.hpp>
#include <ndb/CNdbThreadState.hpp>
#include <ndb/CNdbThreadContext.hpp>
#include <ndb/CNdbThreadWorker.hpp>
#include <ndb/CNdbThreadManager.hpp>

#include <ndb/CNdbThreadContextImpl.hpp>
#include <user/CUserThreadContextImplBuilder.hpp>



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

	CNdbThreadManager NdbThreadManager;
	CUserThreadContextImplBuilder ImplBuilder;
	if (!NdbThreadManager.Init(NdbClusterConnection, ImplBuilder))
		return -1;

	NdbThreadManager.TransitIdleToRun();
	NdbThreadManager.WaitAllThreadState(ETS_Closed);

	return 0;
}
