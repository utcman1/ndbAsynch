template<typename T>
class CNdbThreadManager
{
private:
	int m_MaxThreadWorker = 0;
	std::vector<CNdbThreadWorker<T>*> m_vecWorker;

private:
	bool Release();

	bool CreateAllThread(CNdbClusterConnection& _NdbClusterConnection);
	bool DestroyAllThread();

	bool CheckAllThreadState(const EThreadState _ThreadState) const;
public:
	~CNdbThreadManager();
	bool Init(
		CNdbClusterConnection& _NdbClusterConnection, const int _MaxThreadWorker);

	bool WaitAllThreadState(
		const EThreadState _ThreadState, const int _TimeOutSec = 30) const;
	bool TransitIdleToRun();
};
