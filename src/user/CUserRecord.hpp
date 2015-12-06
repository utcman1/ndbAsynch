struct CTestKey
{
	int a = 0;
};

struct CTestData
{
	int a = 0;
	int b = 0;
};


class NdbTransaction;
class CUserRecordPool;
struct CTestRecord
{
	CTestKey			Key;
	CTestData			Data;
	NdbTransaction*		pTran = nullptr;
	CUserRecordPool*	pUserRecordPool = nullptr;
};



template<>
bool CNdbRecordSpec<CTestKey>::Init();

template<>
bool CNdbRecordSpec<CTestData>::Init();
