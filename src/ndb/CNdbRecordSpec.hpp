// NdbRecordSpec 별로 별도의 템플릿을 정의한다.
// Data Record, Primary Index, Index...
template<typename T>
class CNdbRecordSpec
{
private:
	CNdb& m_Ndb;
	NdbRecord* m_pRecord = nullptr;

private:
	bool Release();

public:
	CNdbRecordSpec(CNdb& _Ndb);
	~CNdbRecordSpec();

	const NdbRecord* GetNdbRecordPtr() const { return m_pRecord; }

public:
	// 유저가 템플릿 타입별로 재정의해야만 하는 함수
	bool Init();
};
