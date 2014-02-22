#include <fm/util/Log.hpp>

namespace zn
{

Log log;

//------------------------------------------------------------------------------
bool Log::openFile(const std::string & fpath)
{
	m_file.open(fpath.c_str());
	if(!m_file.good())
	{
		std::cout << "E: Log: failed to open file \"" << fpath << '"' << std::endl;
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
Log & endl(Log & l)
{
	l << '\n';
	return l;
}

} // namespace zn

