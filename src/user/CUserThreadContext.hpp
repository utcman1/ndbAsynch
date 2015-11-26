class CUserRecordPool;
class CNdbClusterConnection;



class CUserThreadContext
	: public CNdbThreadContext
{
private:
	std::vector<CUserRecordPool*> m_pRecordPoolVector;

private:
	bool CreateRecordPoolVector(CNdbClusterConnection& _NdbClusterConnection);
	void DestroyRecordPoolVector();

	void Release();

public:
	CUserThreadContext();
	virtual ~CUserThreadContext();
	virtual bool Init(CNdbClusterConnection& _NdbClusterConnection);
	CUserRecordPool* GetRecordPool();

	virtual void OnCreate();
	virtual void OnIdle();
	virtual void OnRun();
	virtual void OnClosing();
	virtual void OnDestroy();
};



class CUserThreadContextBuilder
	: public CNdbThreadContextBuilder
{
public:
	virtual ~CUserThreadContextBuilder() {}
	virtual CNdbThreadContext* Create();
};
