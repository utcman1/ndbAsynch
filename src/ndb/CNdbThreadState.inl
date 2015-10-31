template<typename T>
bool CNdbThreadState<T>::Transit(
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



template<typename T>
CNdbThreadState<T>::CNdbThreadState(CNdbClusterConnection& _NdbClusterConnection)
	: CNdb(_NdbClusterConnection)
{
}

template<typename T>
bool CNdbThreadState<T>::Init()
{
	LOG_NDB_FUNCTION();

	if (!CNdb::Init())
		return false;

	if (!T::Init())
		return false;

	return true;
}
