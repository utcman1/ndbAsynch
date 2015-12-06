class CUserRecordPool;
class CNdbClusterConnection;



class CUserRecordPoolManager
{
private:
	std::vector<CUserRecordPool*> m_pRecordPoolVector;

private:
	bool CreateRecordPoolVector(CNdbClusterConnection& _NdbClusterConnection);
	void DestroyRecordPoolVector();

	void Release();

public:
	~CUserRecordPoolManager();
	bool Init(CNdbClusterConnection& _NdbClusterConnection);

	int PrepareAndSendTran(const int _IdxBegin);	// return Prepare Count
	int CompleteTran();								// return Complete Count
};
