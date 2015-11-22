class CNdbClusterConnection;
class CNdbThreadWorker;
class CNdbThreadContextImplBuilder;



class CNdbThreadManager
{
private:
	std::vector<CNdbThreadWorker*> m_vecWorker;

private:
	bool Release();

	bool CreateAllThread(CNdbClusterConnection& _NdbClusterConnection,
		CNdbThreadContextImplBuilder& _ImplBuilder,
		const int _MaxThreadWorker = MaxThreadWorker);
	bool DestroyAllThread();

	bool CheckAllThreadState(const EThreadState _ThreadState) const;
public:
	~CNdbThreadManager();
	bool Init(CNdbClusterConnection& _NdbClusterConnection,
		CNdbThreadContextImplBuilder& _ImplBuilder,
		const int _MaxThreadWorker = MaxThreadWorker);

	bool WaitAllThreadState(
		const EThreadState _ThreadState, const int _TimeOutSec = 30) const;
	bool TransitIdleToRun();
};
