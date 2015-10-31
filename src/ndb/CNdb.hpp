class CNdbClusterConnection;

class CNdb
	: protected Ndb
{
public:
	// CUserThreadState에서 호출할 함수들
	using Ndb::getDictionary;
	using Ndb::setDatabaseName;
	using Ndb::getNdbError;
	using Ndb::startTransaction;
	using Ndb::closeTransaction;

protected:
	// 생성자를 직접 호출할 일이 없다.
	// 상속된 클래스에서 생성자를 호출한다.
	CNdb(CNdbClusterConnection& _NdbClusterConnection);
	bool Init(const int _MaxNoOfTransactions = 256, const int _Timeout = 60);
};
