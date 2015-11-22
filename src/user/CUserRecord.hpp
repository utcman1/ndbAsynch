struct CTestKey
{
	int a = 0;
};

struct CTestData
{
	int a = 0;
	int b = 0;
};


class CUserRecordPool;
struct CTestRecord
{
	NdbTransaction*		pTran = nullptr;
	CTestKey			Key;
	CTestData			Data;
	CUserRecordPool*	pUserRecordPool = nullptr;
};



template<>
bool CNdbRecordSpec<CTestKey>::Init();

template<>
bool CNdbRecordSpec<CTestData>::Init();
