#include <stdafx.hpp>
#include <util/CLog.hpp>
#include <ndb/CNdbThreadState.hpp>



bool CNdbThreadState::Transit(
	const EThreadState _Before, const EThreadState _After)
{
	if (_Before == m_State)
	{
		LogNdbDebug << "ThreadState " << _Before << " => " << _After << endl;
		m_State = _After;
		return true;
	}
	else
	{
		LogNdbError << "Before State[" << m_State
			<< "] is not Match [" << _Before << "]" << endl;
		return false;
	}
}
