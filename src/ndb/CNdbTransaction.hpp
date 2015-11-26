class CNdbTransaction
	: protected NdbTransaction
{
private:
	~CNdbTransaction();

public:
	template<typename TKey, typename TData>
	const NdbOperation* ReadTuple(
		const CNdbRecordSpec<TKey>& _KeyRecordSpec, const TKey& _KeyValue,
		const CNdbRecordSpec<TData>& _DataRecordSpec, TData& _DataValue);

	void WriteTuple();
	void UpdateTuple();
	void InsertTuple();
	void DeleteTuple();

	void ScanTable();
	void ScanIndex();
};



template<typename TKey, typename TData>
const NdbOperation* CNdbTransaction::ReadTuple(
	const CNdbRecordSpec<TKey>& _KeyRecordSpec, const TKey& _KeyValue,
	const CNdbRecordSpec<TData>& _DataRecordSpec, TData& _DataValue)
{
	return NdbTransaction::readTuple(
		_KeyRecordSpec.GetNdbRecordPtr(), reinterpret_cast<const char*>(&_KeyValue),
		_DataRecordSpec.GetNdbRecordPtr(), reinterpret_cast<char*>(&_DataValue));
}
