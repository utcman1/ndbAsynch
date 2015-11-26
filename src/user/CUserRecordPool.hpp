class CNdbClusterConnection;



class CUserRecordPool
	: protected CNdb
{
private:
	CNdbRecordSpec<CTestKey>	m_KeyRecordSpec;
	CNdbRecordSpec<CTestData>	m_DataRecordSpec;
	std::vector<CTestRecord>	m_vecRecordPool;
	std::stack<CTestRecord*>	m_stackFreeRecordPool;

private:
	static void UserAsynchCallback(
		int _Result, NdbTransaction* _pTran, void* _pRecord);

private:
	bool InitNdbDatabse();
	bool InitRecordSpec();
	bool InitRecordPool(const int _RecordPerRecordPool);

	CTestRecord* AllocRecord();
	bool FreeRecord(CTestRecord* _pRecord);

	CTestRecord* EnqueTran(const int _Idx);
	bool DequeTran(CTestRecord* _pRecord);

public:
	CUserRecordPool(CNdbClusterConnection& _NdbClusterConnection);
	bool Init(const int _RecordPerRecordPool = RecordPerRecordPool);

	int PrepareTransactions();
	// CUserThreadContext에서 호출할 함수
	using Ndb::sendPreparedTransactions;
	using Ndb::pollNdb;
};
