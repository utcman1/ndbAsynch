class CNdbClusterConnection;
class CNdbThreadContextImplBuilder;



class CNdbThreadWorker
	: public CNdbThreadState, public CNdbThreadContext
{
private:
	NdbThread* m_pThread = nullptr;

private:
	static void* StaticMain(void* _param);

	bool CreateThread();
	bool DestroyThread();

	void Release();

public:
	CNdbThreadWorker(CNdbClusterConnection& _NdbClusterConnection);
	~CNdbThreadWorker();
	bool Init(CNdbThreadContextImplBuilder& _ImplBuilder);

	void* WorkerMain();
};
