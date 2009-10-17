#include "logger.hpp"

std::ofstream * Logger::strmLog=new std::ofstream();;
int Logger::counter=0;

void Logger::WriteLog(std::string msg)
{
	std::cout<<msg<<std::endl;
	(*strmLog)<<msg<<"\n";
}



Logger::Logger()
{
	if (counter==0)
	{
		(*strmLog).open("pbanki.log",std::ios_base::app);
	//	(*strmLog)<<"log created\n";
	}
	
	counter ++;
	
}

Logger::~Logger()
{
	counter --;
	//(*strmLog)<<"log closed\n";
	if (counter==0)
	{
		(*strmLog).close();
	}
}
