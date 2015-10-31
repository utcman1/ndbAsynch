#include <stdafx.hpp>
#include <util/CLog.hpp>
#include <ndb/CNdbInit.hpp>



CNdbInit::CNdbInit()
{
	// ndb_init()이 호출되지 않으면 로그도 정상동작하지 못하기 때문에
	// 이곳의 로그는 특별처리가 필요
	// LOG_NDB_FUNCTION();

	// ndb_init의 리턴값은 항상 0으로 고정이기 때문에 검증할 필요가 없다
	ndb_init();
	LOG_NDB_FUNCTION();
	NDB_CALL_FUNCTION(ndb_init());
}

CNdbInit::~CNdbInit()
{
	LOG_NDB_FUNCTION();

	// ndb_end의 리턴값은 void 형식
	NDB_CALL_FUNCTION(ndb_end(0));
}
