class CNdbClusterConnection;



class CUserThreadContext
	: public CNdbThreadContext
{
private:
	CNdbThreadState& m_NdbThreadState;
	CUserRecordPoolManager m_RecordPoolManager;

private:
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
