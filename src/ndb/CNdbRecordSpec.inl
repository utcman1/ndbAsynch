template<typename T>
bool CNdbRecordSpec<T>::Release()
{
	LOG_NDB_FUNCTION();

	if (nullptr != m_pRecord)
	{
		NdbDictionary::Dictionary* pDict = m_Ndb.getDictionary();

		NDB_CALL_FUNCTION(pDict->releaseRecord(m_pRecord));
		m_pRecord = nullptr;
	}

	return true;
}

template<typename T>
CNdbRecordSpec<T>::CNdbRecordSpec(CNdb& _Ndb)
	: m_Ndb(_Ndb)
{
	// 로그 생략
	//LOG_NDB_FUNCTION();
}

template<typename T>
CNdbRecordSpec<T>::~CNdbRecordSpec()
{
	// 로그 생략
	//LOG_NDB_FUNCTION();

	CNdbRecordSpec::Release();
}
