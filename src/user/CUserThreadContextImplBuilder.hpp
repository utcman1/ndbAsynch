class CUserThreadContextImplBuilder
	: public CNdbThreadContextImplBuilder
{
public:
	virtual ~CUserThreadContextImplBuilder() {}
	virtual CNdbThreadContextImpl* Create(CNdbThreadContext* _pContext);
};
