extern NdbOut& __LogNdbDebug;
extern NdbOut& __LogNdbWarning;
extern NdbOut& __LogNdbError;
extern NdbOut& __LogNdbCritical;

extern NdbOut& __LogUserDebug;
extern NdbOut& __LogUserWarning;
extern NdbOut& __LogUserError;
extern NdbOut& __LogUserCritical;



// NdbOut << std::chrono::system_clock::now() 지원
NdbOut& operator<<(NdbOut& _NdbOut,
	const std::chrono::time_point < std::chrono::system_clock,
		std::chrono::system_clock::duration >& _Now);



class CLogNdbFunction
{
private:
	const char* const	m_SrcFile = nullptr;
	const int			m_SrcLine = 0;
	const char* const	m_SrcFunc = nullptr;

public:
	CLogNdbFunction(const char* const _SrcFile, const int _SrcLine, const char* const _SrcFunc)
		: m_SrcFile(_SrcFile), m_SrcLine(_SrcLine), m_SrcFunc(_SrcFunc)
	{
		__LogNdbDebug << std::chrono::system_clock::now() << " [NDB DEBUG]  (" << m_SrcFile << ":" << m_SrcLine << ")  - [FUNC_BGN]  " << m_SrcFunc << endl;
	}

	~CLogNdbFunction()
	{
		__LogNdbDebug << std::chrono::system_clock::now() << " [NDB DEBUG]  (" << m_SrcFile << ":" << m_SrcLine << ")  - [FUNC_END]  " << m_SrcFunc << endl;
	}
};

class CLogUserFunction
{
private:
	const char* const	m_SrcFile = nullptr;
	const int			m_SrcLine = 0;
	const char* const	m_SrcFunc = nullptr;

public:
	CLogUserFunction(const char* const _SrcFile, const int _SrcLine, const char* const _SrcFunc)
		: m_SrcFile(_SrcFile), m_SrcLine(_SrcLine), m_SrcFunc(_SrcFunc)
	{
		__LogUserDebug << std::chrono::system_clock::now() << " [USER DEBUG]  (" << m_SrcFile << ":" << m_SrcLine << ")  - [FUNC_BGN]  " << m_SrcFunc << endl;
	}

	~CLogUserFunction()
	{
		__LogUserDebug << std::chrono::system_clock::now() << " [USER DEBUG]  (" << m_SrcFile << ":" << m_SrcLine << ")  - [FUNC_END]  " << m_SrcFunc << endl;
	}
};




#ifdef _WIN32
	#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

#define LogNdbDebug					__LogNdbDebug		<< std::chrono::system_clock::now() << " [NDB DEBUG]  ("		<< __FILE__ << ":" << __LINE__ << ")  - "
#define LogNdbWarning				__LogNdbWarning		<< std::chrono::system_clock::now() << " [NDB WARNING]  ("	<< __FILE__ << ":" << __LINE__ << ")  - "
#define LogNdbError					__LogNdbError		<< std::chrono::system_clock::now() << " [NDB ERROR]  ("		<< __FILE__ << ":" << __LINE__ << ")  - "
#define LogNdbCritical				__LogNdbCritical	<< std::chrono::system_clock::now() << " [NDB CRITICAL]  ("	<< __FILE__ << ":" << __LINE__ << ")  - "

#define LogUserDebug				__LogUserDebug		<< std::chrono::system_clock::now() << " [USER DEBUG]  ("	<< __FILE__ << ":" << __LINE__ << ")  - "
#define LogUserWarning				__LogUserWarning	<< std::chrono::system_clock::now() << " [USER WARNING]  ("	<< __FILE__ << ":" << __LINE__ << ")  - "
#define LogUserError				__LogUserError		<< std::chrono::system_clock::now() << " [USER ERROR]  ("	<< __FILE__ << ":" << __LINE__ << ")  - "
#define LogUserCritical				__LogUserCritical	<< std::chrono::system_clock::now() << " [USER CRITICAL]  ("	<< __FILE__ << ":" << __LINE__ << ")  - "

#ifdef _DEBUG
	#define LOG_NDB_FUNCTION()		CLogNdbFunction		LogNdbFunction(__FILE__, __LINE__, __PRETTY_FUNCTION__)
	#define LOG_USER_FUNCTION()		CLogUserFunction	LogUserFunction(__FILE__, __LINE__, __PRETTY_FUNCTION__)
	#define NDB_CALL_FUNCTION(F)	(__LogNdbDebug		<< std::chrono::system_clock::now() << " [NDB DEBUG]  ("	<< __FILE__ << ":" << __LINE__ << ")  - [NDB_CALL]  " << #F << endl,(F))
	#define USER_CALL_FUNCTION(F)	(__LogUserDebug		<< std::chrono::system_clock::now() << " [USER DEBUG]  ("	<< __FILE__ << ":" << __LINE__ << ")  - [NDB_CALL]  " << #F << endl,(F))
#else
	#define LOG_NDB_FUNCTION()		
	#define LOG_USER_FUNCTION()		
	#define NDB_CALL_FUNCTION(F)	F
	#define USER_CALL_FUNCTION(F)	F
#endif
