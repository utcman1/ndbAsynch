class CNdbThreadContext;



class CUserThreadContextImpl
	: public CNdbThreadContextImpl, public CUserRecordPool
{
private:
	CNdbThreadContext* m_pContext;

public:
	CUserThreadContextImpl(CNdbThreadContext* _pContext)
		: m_pContext(_pContext) {}
	virtual ~CUserThreadContextImpl() {}

	virtual void OnCreate();
	virtual void OnIdle();
	virtual void OnRun();
	virtual void OnClosing();
	virtual void OnDestroy();
};
