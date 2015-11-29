class CUserRecordPool;
class CNdbClusterConnection;



class CUserThreadContext
	: public CNdbThreadContext
{
private:
	CNdbThreadState& m_NdbThreadState;
	std::vector<CUserRecordPool*> m_pRecordPoolVector;

private:
	bool CreateRecordPoolVector(CNdbClusterConnection& _NdbClusterConnection);
	void DestroyRecordPoolVector();

	void Release();

public:
	CUserThreadContext(CNdbThreadState& _NdbThreadState);
	virtual ~CUserThreadContext();
	virtual bool Init(CNdbClusterConnection& _NdbClusterConnection);

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
	virtual CNdbThreadContext* Create(CNdbThreadState& _NdbThreadState);
};
