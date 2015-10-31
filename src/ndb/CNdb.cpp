#include <stdafx.hpp>
#include <util/CLog.hpp>
#include <ndb/CNdbClusterConnection.hpp>
#include <ndb/CNdb.hpp>



CNdb::CNdb(CNdbClusterConnection& _NdbClusterConnection)
	: Ndb(&_NdbClusterConnection)
{
	LOG_NDB_FUNCTION();
}

bool CNdb::Init(const int _MaxNoOfTransactions, const int _Timeout)
{
	LOG_NDB_FUNCTION();

	if (0 != NDB_CALL_FUNCTION(Ndb::init(_MaxNoOfTransactions)))
	{
		LogNdbCritical << Ndb::getNdbError() << endl;
		return false;
	}

	if (0 != NDB_CALL_FUNCTION(Ndb::waitUntilReady(_Timeout)))
	{
		LogNdbCritical << Ndb::getNdbError() << endl;
		return false;
	}

	return true;
}
