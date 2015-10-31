struct CTestKey
{
	int a = 0;
};

struct CTestData
{
	int a = 0;
	int b = 0;
};


class CUserThreadState;
struct CTestRecord
{
	NdbTransaction*		pTran = nullptr;
	CTestKey			Key;
	CTestData			Data;
	CUserThreadState*	pUserThreadState = nullptr;
};



template<>
bool CNdbRecordSpec<CTestKey>::Init();

template<>
bool CNdbRecordSpec<CTestData>::Init();
