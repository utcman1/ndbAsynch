class CNdbClusterConnection;
class CNdbThreadContext;
class CNdbThreadContextBuilder;



class CNdbThreadWorker
	: public CNdbThreadState
{
private:
	NdbThread* m_pThread = nullptr;
	CNdbThreadContext* m_pContext = nullptr;

private:
	static void* StaticMain(void* _param);

	bool CreateContext(CNdbClusterConnection& _NdbClusterConnection,
		CNdbThreadContextBuilder& _Builder);
	void DestroyContext();

	bool CreateThread();
	void DestroyThread();

	void Release();

public:
	CNdbThreadWorker();
	~CNdbThreadWorker();
	bool Init(CNdbClusterConnection& _NdbClusterConnection,
		CNdbThreadContextBuilder& _Builder);

	void* WorkerMain();
};
