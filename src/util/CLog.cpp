#include <stdafx.hpp>
#include <util/CLog.hpp>



NullOutputStream __NullOutputStream;
NdbOut nullout(__NullOutputStream);



NdbOut& __LogNdbDebug		=
#ifdef _DEBUG
ndbout;
#else
nullout;
#endif

NdbOut& __LogNdbWarning		= ndbout;
NdbOut& __LogNdbError		= ndberr;
NdbOut& __LogNdbCritical	= ndberr;



NdbOut& __LogUserDebug		=
#ifdef _DEBUG
ndbout;
#else
nullout;
#endif

NdbOut& __LogUserWarning	= ndbout;
NdbOut& __LogUserError		= ndberr;
NdbOut& __LogUserCritical	= ndberr;



NdbOut& operator<<(NdbOut& _NdbOut,
	const std::chrono::time_point < std::chrono::system_clock,
		std::chrono::system_clock::duration >& _Now)
{
	std::time_t t = std::chrono::system_clock::to_time_t(_Now);
	char StrBuf[32];

	// '2000-01-01 00:00:00.' : 20자리 필요
	std::tm tm;
	localtime_r(&t, &tm);
	strftime(StrBuf, sizeof(StrBuf), "%Y-%m-%d %H:%M:%S.", &tm);
	_NdbOut << StrBuf;

	// milli second 출력
	long long llMillisec = std::chrono::duration_cast<std::chrono::milliseconds>(_Now.time_since_epoch()).count() % 1000;
	int millisec = static_cast<int>(llMillisec);
	sprintf(StrBuf, "%03d", millisec);
	_NdbOut << StrBuf;

	return _NdbOut;
}
