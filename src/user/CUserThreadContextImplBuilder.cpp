#include <stdafx.hpp>
#include <util/CLog.hpp>
#include <ndb/CNdbThreadContextImpl.hpp>
#include <user/CUserThreadContextImplBuilder.hpp>

#include <ndb/CNdb.hpp>
#include <ndb/CNdbRecordSpec.hpp>
#include <user/CUserRecord.hpp>
#include <user/CUserRecordPool.hpp>
#include <user/CUserThreadContextImpl.hpp>


CNdbThreadContextImpl* CUserThreadContextImplBuilder::Create(CNdbThreadContext* _pContext)
{
	LOG_USER_FUNCTION();

	return new CUserThreadContextImpl(_pContext);
}
