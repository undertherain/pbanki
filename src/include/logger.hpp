#ifndef MINDCRAFT_LOGGER
#define MINDCRAFT_LOGGER


#include <iostream>
#include <fstream>
#include <string>


class Logger
{
private:
	static std::ofstream * strmLog;
	static int counter;

public:
	Logger();
	~Logger();
	static void WriteLog(std::string msg);
};


#endif //MINDCRAFT_LOGGER
