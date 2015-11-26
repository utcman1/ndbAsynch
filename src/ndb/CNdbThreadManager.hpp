class CNdbClusterConnection;
class CNdbThreadWorker;
class CNdbThreadContextBuilder;



class CNdbThreadManager
{
private:
	std::vector<CNdbThreadWorker*> m_vecWorker;

private:
	bool CreateAllThread(CNdbClusterConnection& _NdbClusterConnection,
		CNdbThreadContextBuilder& _Builder,
		const int _MaxThreadWorker = MaxThreadWorker);
	bool DestroyAllThread();

	void Release();

	bool CheckAllThreadState(const EThreadState _ThreadState) const;

public:
	~CNdbThreadManager();
	bool Init(CNdbClusterConnection& _NdbClusterConnection,
		CNdbThreadContextBuilder& _Builder,
		const int _MaxThreadWorker = MaxThreadWorker);

	bool WaitAllThreadState(
		const EThreadState _ThreadState, const int _TimeOutSec = 30) const;
	bool TransitIdleToRun();
};
