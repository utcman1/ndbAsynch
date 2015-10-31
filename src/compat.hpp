#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#define localtime_r(time_t,tm) localtime_s(tm,time_t)
#define sprintf sprintf_s
#endif
