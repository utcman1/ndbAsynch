class CNdbClusterConnection;
class CNdb;
class CNdbThreadContextImplBuilder;
class CNdbThreadContextImpl;



class CNdbThreadContext
{
private:
	CNdbClusterConnection& m_NdbClusterConnection;
	CNdb* m_pArrNdb[MaxNdbPerThread];
	CNdbThreadContextImpl* m_pImpl = nullptr;

private:
	bool CreateNdbPool();
	bool DestroyNdbPool();

	bool CreateImpl(CNdbThreadContextImplBuilder& _ImplBuilder);
	bool DestroyImpl();

	bool Release();

public:
	CNdbThreadContext(CNdbClusterConnection& _NdbClusterConnection);
	~CNdbThreadContext();
	bool Init(CNdbThreadContextImplBuilder& _ImplBuilder);
	CNdb* GetNdb() { return m_pArrNdb[0]; }

public:
	void OnCreate();
	void OnIdle();
	void OnRun();
	void OnClosing();
	void OnDestroy();
};
