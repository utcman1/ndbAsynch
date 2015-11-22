class CNdbClusterConnection;



class CNdb
	: protected Ndb
{
public:
	// CUser*에서 호출할 함수들
	using Ndb::getDictionary;
	using Ndb::setDatabaseName;
	using Ndb::getNdbError;
	using Ndb::startTransaction;
	using Ndb::closeTransaction;
	using Ndb::sendPollNdb;

public:
	// 이 함수는 CNdbThreadWorker에서만 호출할거다
	CNdb(CNdbClusterConnection& _NdbClusterConnection);
	bool Init(const int _MaxNoOfTransactions = MaxTransactionPerNdb, const int _Timeout = 60);
};
