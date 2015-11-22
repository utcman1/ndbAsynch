class CUserThreadContextImplBuilder
	: public CNdbThreadContextImplBuilder
{
public:
	virtual CNdbThreadContextImpl* Create(CNdbThreadContext* _pContext);
};
