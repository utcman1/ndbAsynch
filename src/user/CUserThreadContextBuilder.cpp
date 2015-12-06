#include <stdafx.hpp>
#include <util/CLog.hpp>
#include <ndb/CNdbThreadContext.hpp>
#include <user/CUserRecordPoolManager.hpp>
#include <user/CUserThreadContext.hpp>
#include <user/CUserThreadContextBuilder.hpp>



CNdbThreadContext* CUserThreadContextBuilder::Create(
	CNdbThreadState& _NdbThreadState)
{
	LOG_USER_FUNCTION();

	return new CUserThreadContext(_NdbThreadState);
}
