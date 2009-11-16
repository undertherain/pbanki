//description here

#include "inkview.h"

#include "include/view_pb.hpp"
#include "include/logger.hpp"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char * argv[])
{
#ifdef ARCH_PB

	chdir("/mnt/ext1/games/");
	int fd1 = open("log1.txt",O_CREAT|0666);
	dup2(fd1, 1); 

	int fd2 = open("log2.txt",O_CREAT|0666);
	dup2(fd2, 2); 
#endif
	Logger logger;
	logger.WriteLog("Hello, pocketbook");
	
#ifdef ARCH_PB
	close(fd1);
	close(fd2);
#endif

	InkViewMain(mainHandler);
	return 0;
}

