template<typename T>
class CNdbThreadWorker
	: public CNdbThreadState<T>
{
	static_assert(std::is_base_of<CNdb, CNdbThreadState<T>>::value,
		"CNdbThreadState<T> must derive from CNdb");
	static_assert(std::is_base_of<T, CNdbThreadState<T>>::value,
		"CNdbThreadState<T> must derive from T");

private:
	NdbThread* m_pThread = nullptr;

private:
	static void* StaticMain(void* _param);

	bool Release();

	bool CreateThread();
	bool DestroyThread();

public:
	CNdbThreadWorker(CNdbClusterConnection& _NdbClusterConnection);
	~CNdbThreadWorker();

	bool Init();

	void* WorkerMain();
};
