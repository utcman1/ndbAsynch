class CUserThreadContextBuilder
	: public CNdbThreadContextBuilder
{
public:
	virtual ~CUserThreadContextBuilder() {}
	virtual CNdbThreadContext* Create(CNdbThreadState& _NdbThreadState);
};
