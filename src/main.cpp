#include <stdafx.hpp>
#include <ndb/CNdbInit.hpp>
#include <ndb/CNdbClusterConnection.hpp>
#include <ndb/CNdbThreadState.hpp>
#include <ndb/CNdbThreadManager.hpp>

#include <ndb/CNdbThreadContext.hpp>
#include <user/CUserThreadContextBuilder.hpp>



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
	CUserThreadContextBuilder ContextBuilder;
	if (!NdbThreadManager.Init(NdbClusterConnection, ContextBuilder))
		return -1;

	NdbThreadManager.TransitIdleToRun();
	NdbThreadManager.WaitAllThreadState(ETS_Closed, 3600);

	return 0;
}
