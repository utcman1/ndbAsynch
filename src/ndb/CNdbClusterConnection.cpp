#include <stdafx.hpp>
#include <util/CLog.hpp>
#include <util/CMysqlConfig.hpp>
#include <ndb/CNdbClusterConnection.hpp>



CNdbClusterConnection::CNdbClusterConnection(
	const CNdbInit&, const int _NodeID)
	: Ndb_cluster_connection(
		CMysqlConfig::Get("server", "ndb-connectstring").c_str(), _NodeID)
{
	LOG_NDB_FUNCTION();
}

CNdbClusterConnection::CNdbClusterConnection(
	const CNdbInit&, const int _NodeID, const char* _pNdbConnectString)
	: Ndb_cluster_connection(_pNdbConnectString, _NodeID)
{
	LOG_NDB_FUNCTION();
}

CNdbClusterConnection::~CNdbClusterConnection()
{
	LOG_NDB_FUNCTION();
}

bool CNdbClusterConnection::Init(const int _TimeoutForFirstAlive,
	const int _TimeoutAfterFirstAlive)
{
	LOG_NDB_FUNCTION();

	if (0 != NDB_CALL_FUNCTION(Ndb_cluster_connection::connect()))
	{
		LogNdbCritical << Ndb_cluster_connection::get_latest_error_msg() << endl;
		return false;
	}

	if (0 != NDB_CALL_FUNCTION(Ndb_cluster_connection::wait_until_ready(
		_TimeoutForFirstAlive, _TimeoutAfterFirstAlive)))
	{
		LogNdbCritical << Ndb_cluster_connection::get_latest_error_msg() << endl;
		return false;
	}

	return true;
}
