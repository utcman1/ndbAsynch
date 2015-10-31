#include <stdafx.hpp>
#include <util/CMysqlConfig.hpp>
#include <util/CPopen.hpp>



std::string CMysqlConfig::Get(const std::string& _Section, const std::string& _Key)
{
	std::string CmdStr = "my_print_defaults " + _Section + " | grep " + _Key;
	std::vector<std::string> Results = CPopen::GetResults(CmdStr);

	// my_print_defaults의 동작으로 인해 설정이 중복으로 리턴되지 않으며 주석도 알아서 없어진다.
	for (auto line : Results)
	{
		std::size_t EqualPos = line.find("=");
		if (std::string::npos != EqualPos)
			return line.substr(EqualPos+1, std::string::npos);
	}

	return std::string();
}
