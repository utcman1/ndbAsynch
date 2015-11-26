class CNdbClusterConnection;



class CNdb
	: protected Ndb
{
public:
	// CUserRecord에서 호출할 함수
	using Ndb::getDictionary;

public:
	// 이 함수는 CNdbThreadWorker에서만 호출할거다
	CNdb(CNdbClusterConnection& _NdbClusterConnection);
	bool Init(const int _MaxNoOfTransactions = MaxRecordPoolSize, const int _Timeout = 60);
};
