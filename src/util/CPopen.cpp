#include <stdafx.hpp>
#include <util/CPopen.hpp>



struct CustomDeleter
{
	void operator()(FILE* _fp) { pclose(_fp); }
};



// 호출 빈도가 매우 낮기 때문에
// 성능에 대해 고민할 필요가 없는 함수
std::vector<std::string> CPopen::GetResults(const std::string& _cmd)
{
	std::vector<std::string> results;

	std::unique_ptr<FILE, CustomDeleter> PopenObj(popen(_cmd.c_str(), "r"));
	if (!PopenObj)
		return results;

	char linebuf[4096];

	while(fgets(linebuf, sizeof(linebuf), PopenObj.get()))
	{
		std::string line(linebuf);
		std::size_t EndlPos = line.find("\n");
		if (std::string::npos != EndlPos)
			line = line.substr(0, EndlPos);
		results.push_back(line);
	}

	return results;
}
