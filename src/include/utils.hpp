#include <ctime>
#include <sstream>


class FormatHelper 
{
public:
	static std::string GetTimeStr()
	{
		time_t seconds;
		seconds = time (NULL);
		std::ostringstream out;
		out << seconds;
		std::string str = out.str();
		return str;

	}
	template <typename T>
	static std::string ConvertToStr(const T & val)
	{
		std::ostringstream out;
		out << val;
		std::string str = out.str();
		return str;
	}
	static int StrToInt(const std::string & str)
	{
		int value;
		std::istringstream tempStream(str);
		tempStream>>value;
		return value;
	}
};
