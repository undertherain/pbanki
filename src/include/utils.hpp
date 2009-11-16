
#ifndef MINDCRAFT_UTILS
#define MINDCRAFT_UTILS


#include <ctime>
#include <sstream>
#ifdef ARCH_WM
#include <winbase.h>
#endif

class TimeHelper
{
public:
	static long GetSeconds()
	{
		time_t seconds;

		#ifdef ARCH_WM
		seconds =_time64(NULL);
		#else
		seconds = time(NULL);
		#endif
		return seconds;
	}

};

class FormatHelper 
{
public:
	static std::string GetCurrentTimeStr()
	{

		
		std::ostringstream out;
		out << TimeHelper::GetSeconds();
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
