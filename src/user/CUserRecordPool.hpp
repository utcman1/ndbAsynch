class CUserRecordPool
{
protected:
	CNdbRecordSpec<CTestKey>*	m_pKeyRecordSpec = nullptr;
	CNdbRecordSpec<CTestData>*	m_pDataRecordSpec = nullptr;
	std::vector<CTestRecord>	m_vecRecordPool;
	std::stack<CTestRecord*>	m_stackFreeRecordPool;

protected:
	static void UserAsynchCallback(int _Result, NdbTransaction* _pTran, void* _pRecord);

protected:
	bool InitRecordSpec(CNdb& _Ndb);
	bool InitRecordPool();
	bool InitOnCreate(CNdb& _Ndb);

	CTestRecord* AllocRecord();
	bool FreeRecord(CTestRecord* _pRecord);

	CTestRecord* EnqueTran(CNdb& _Ndb, const int _Num);
	bool DequeTran(CTestRecord* _pRecord);

	int EnqueLoop(CNdb& _Ndb);

public:
	bool Init()
	{
		return true;
	}
};
