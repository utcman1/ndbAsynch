class CNdbClusterConnection
	: protected Ndb_cluster_connection
{
friend class CNdb;	// Ndb_cluster_connection*에 접근이 필요해서 열어줬다.

public:
	CNdbClusterConnection(const class CNdbInit&, const int _NodeID);
	CNdbClusterConnection(const class CNdbInit&, const int _NodeID,
		const char* _pNdbConnectString);
	~CNdbClusterConnection();

	bool Init(const int _TimeoutForFirstAlive = 30,
		const int _TimeoutAfterFirstAlive = 30);
};
