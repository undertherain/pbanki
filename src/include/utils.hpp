
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
#if  defined(ARCH_WM) || defined(ARCH_WIN)
	static void wstrToUtf8(std::string& dest, const std::wstring& src){
		dest.clear();
		for (size_t i = 0; i < src.size(); i++){
			wchar_t w = src[i];
			if (w <= 0x7f)
				dest.push_back((char)w);
			else if (w <= 0x7ff){
				dest.push_back(0xc0 | ((w >> 6)& 0x1f));
				dest.push_back(0x80| (w & 0x3f));
			}
			else if (w <= 0xffff){
				dest.push_back(0xe0 | ((w >> 12)& 0x0f));
				dest.push_back(0x80| ((w >> 6) & 0x3f));
				dest.push_back(0x80| (w & 0x3f));
			}
			else if (w <= 0x10ffff){
				dest.push_back(0xf0 | ((w >> 18)& 0x07));
				dest.push_back(0x80| ((w >> 12) & 0x3f));
				dest.push_back(0x80| ((w >> 6) & 0x3f));
				dest.push_back(0x80| (w & 0x3f));
			}
			else
				dest.push_back('?');
		}
	}
#endif
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
