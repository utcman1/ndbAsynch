#include <stdafx.hpp>
#include <util/CLog.hpp>
#include <ndb/CNdb.hpp>
#include <ndb/CNdbThreadContextImpl.hpp>
#include <ndb/CNdbThreadContext.hpp>



bool CNdbThreadContext::CreateNdbPool()
{
	LOG_NDB_FUNCTION();

	for (CNdb*& pNdb : m_pArrNdb)
	{
		pNdb = new CNdb(m_NdbClusterConnection);
		assert(nullptr != pNdb);

		if (!pNdb->Init())
			return false;
	}

	return true;
}

bool CNdbThreadContext::DestroyNdbPool()
{
	LOG_NDB_FUNCTION();

	for (CNdb*& pNdb : m_pArrNdb)
	{
		delete pNdb;
		pNdb = nullptr;
	}

	return true;
}

bool CNdbThreadContext::CreateImpl(CNdbThreadContextImplBuilder& _ImplBuilder)
{
	LOG_NDB_FUNCTION();

	assert(nullptr == m_pImpl);
	m_pImpl = _ImplBuilder.Create(this);

	return (nullptr != m_pImpl);
}

bool CNdbThreadContext::DestroyImpl()
{
	LOG_NDB_FUNCTION();

	delete m_pImpl;
	m_pImpl = nullptr;

	return true;
}

void CNdbThreadContext::Release()
{
	LOG_NDB_FUNCTION();

	CNdbThreadContext::DestroyImpl();
	CNdbThreadContext::DestroyNdbPool();
}



CNdbThreadContext::CNdbThreadContext(
	CNdbClusterConnection& _NdbClusterConnection)
	: m_NdbClusterConnection(_NdbClusterConnection)
{
	LOG_NDB_FUNCTION();
}

CNdbThreadContext::~CNdbThreadContext()
{
	LOG_NDB_FUNCTION();

	CNdbThreadContext::Release();
}

bool CNdbThreadContext::Init(CNdbThreadContextImplBuilder& _ImplBuilder)
{
	LOG_NDB_FUNCTION();

	if (!CNdbThreadContext::CreateNdbPool())
		return false;

	if (!CNdbThreadContext::CreateImpl(_ImplBuilder))
		return false;

	return true;
}

void CNdbThreadContext::OnCreate()	{ m_pImpl->OnCreate(); }
void CNdbThreadContext::OnIdle()	{ m_pImpl->OnIdle(); }
void CNdbThreadContext::OnRun()		{ m_pImpl->OnRun(); }
void CNdbThreadContext::OnClosing()	{ m_pImpl->OnClosing(); }
void CNdbThreadContext::OnDestroy()	{ m_pImpl->OnDestroy(); }
