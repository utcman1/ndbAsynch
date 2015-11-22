class CNdbThreadContextImpl
{
public:
	// OnCreate(), OnDestroy() 함수는 1번만 불리는게 보장된다.
	virtual void OnCreate() = 0;	// worker 생성후 호출됨
	virtual void OnIdle() = 0;		// Idle 상태에서 update될때 호출됨
	virtual void OnRun() = 0;		// Run 상태에서 update될때 호출됨
	virtual void OnClosing() = 0;	// Closing 상태에서 update될때 호출됨. 처리가 완료되면 ETS_Destroy로 상태를 바꿔줘야 한다.
	virtual void OnDestroy() = 0;	// worker 파괴되기 전 호출됨
};



class CNdbThreadContext;
class CNdbThreadContextImplBuilder
{
public:
	virtual CNdbThreadContextImpl* Create(CNdbThreadContext* _pContext) = 0;
};
