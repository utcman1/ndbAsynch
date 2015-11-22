#include <stdafx.hpp>
#include <util/CLog.hpp>
#include <ndb/CNdb.hpp>
#include <ndb/CNdbRecordSpec.hpp>
#include <user/CUserRecord.hpp>



template<>
bool CNdbRecordSpec<CTestKey>::Init()
{
	LOG_USER_FUNCTION();

	auto pDict = m_Ndb.getDictionary();
	assert(nullptr != pDict);

	auto pTable = USER_CALL_FUNCTION(pDict->getTable("t_ndb_test"));
	if (nullptr == pTable)
	{
		LogUserError << pDict->getNdbError() << endl;
		return false;
	}

	NdbDictionary::RecordSpecification RecSpec;

	RecSpec.column = pTable->getColumn("a");
	RecSpec.offset = offsetof(CTestKey, a);
	RecSpec.nullbit_byte_offset = 0;
	RecSpec.nullbit_bit_in_byte = 0;
	RecSpec.column_flags = 0;

	m_pRecord = USER_CALL_FUNCTION(pDict->createRecord(pTable, &RecSpec, 1, sizeof(RecSpec)));
	if (nullptr == m_pRecord)
	{
		LogUserError << pDict->getNdbError() << endl;
		return false;
	}

	return true;
}

template<>
bool CNdbRecordSpec<CTestData>::Init()
{
	LOG_USER_FUNCTION();

	auto pDict = m_Ndb.getDictionary();
	assert(nullptr != pDict);

	auto pTable = USER_CALL_FUNCTION(pDict->getTable("t_ndb_test"));
	if (nullptr == pTable)
	{
		LogUserError << pDict->getNdbError() << endl;
		return false;
	}

	NdbDictionary::RecordSpecification RecSpec[2];

	RecSpec[0].column = pTable->getColumn("a");
	RecSpec[0].offset = offsetof(CTestData, a);
	RecSpec[0].nullbit_byte_offset = 0;
	RecSpec[0].nullbit_bit_in_byte = 0;
	RecSpec[0].column_flags = 0;

	RecSpec[1].column = pTable->getColumn("b");
	RecSpec[1].offset = offsetof(CTestData, b);
	RecSpec[1].nullbit_byte_offset = 0;
	RecSpec[1].nullbit_bit_in_byte = 0;
	RecSpec[1].column_flags = 0;

	m_pRecord = USER_CALL_FUNCTION(pDict->createRecord(pTable, &RecSpec[0], 2, sizeof(RecSpec[0])));
	if (nullptr == m_pRecord)
	{
		LogUserError << pDict->getNdbError() << endl;
		return false;
	}

	return true;
}
