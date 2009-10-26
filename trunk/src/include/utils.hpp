
#ifndef MINDCRAFT_UTILS
#define MINDCRAFT_UTILS


#include <ctime>
#include <sstream>


class FormatHelper 
{
public:
	static std::string GetCurrentTimeStr()
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
	static float StrToFloat(const std::string & str)
	{
		float value;
		std::istringstream tempStream(str);
		tempStream>>value;
		return value;
	}
};
#endif //MINDCRAFT_UTILS
