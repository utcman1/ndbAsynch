enum EThreadState
{
	ETS_Invalid,		// 초기화 전
	ETS_Create,			// Worker Thread Create된 상태 ~ WorkerMail()의 OnCreate()를 호출할때까지
						// Worker Thread가 동작하기 전 초기화를 위한 상태
	ETS_Idle,			// ThreadWorker가 대기중인 상태
	ETS_Run,			// ThreadWorker가 동작중인 상태
	ETS_Closing,		// ThreadWorker가 종료중인 상태. 진행중이던 Transaction에 대한 cleanup 처리를 해야 한다.
	ETS_Destroy,		// ThreadWorker가 종료완료된 상태. ETS_End에서 Transaction cleanup이 완료되면 이 상태로 전환한다.
						// ETS_Create의 반대로 Worker Thread가 정지하기 전 해제 처리를 한다.
	ETS_Closed,			// Worker Thread가 완전히 종료되어 삭제 가능한 상태.
};



template<typename T>
class CNdbThreadState
	: public CNdb, public T
{
private:
	EThreadState m_State = ETS_Invalid;

private:
	bool Transit(const EThreadState _Before, const EThreadState _After);

protected:
	// 생성자 / Init() 함수는 CNdbThreadWorker에서 접근한다.
	CNdbThreadState(CNdbClusterConnection& _NdbClusterConnection);
	bool Init();

public:
	// 상태 관리 함수는 외부에서도 접근한다.
	EThreadState GetState() const { return m_State; }
	bool TransitInvalidToCreate()	{ return Transit(ETS_Invalid, ETS_Create); }
	bool TransitCreateToIdle()		{ return Transit(ETS_Create, ETS_Idle); }
	bool TransitIdleToRun()			{ return Transit(ETS_Idle, ETS_Run); }
	bool TransitRunToIdle()			{ return Transit(ETS_Run, ETS_Idle); }
	bool TransitRunToClosing()		{ return Transit(ETS_Run, ETS_Closing); }
	bool TransitClosingToDestroy()	{ return Transit(ETS_Closing, ETS_Destroy); }
	bool TransitDestroyToClosed()	{ return Transit(ETS_Destroy, ETS_Closed); }

protected:
	// template specialization을 통해 유저가 구현해야 하는 함수
	// OnCreate(), OnDestroy() 함수는 1번만 불리는게 보장된다.
	void OnCreate();	// worker 생성후 호출됨
	void OnIdle();		// Idle 상태에서 update될때 호출됨
	void OnRun();		// Run 상태에서 update될때 호출됨
	void OnClosing();	// Closing 상태에서 update될때 호출됨. 처리가 완료되면 ETS_Destroy로 상태를 바꿔줘야 한다.
	void OnDestroy();	// worker 파괴되기 전 호출됨
};
