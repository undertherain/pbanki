#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mount.h>
#include <dlfcn.h>

//#include <iostream>
//#include <string>

//#include <dirent.h>
//#include <sys/types.h>

#include "inkview.h"

#include "view_pb.hpp"

class tmp
{
public:
	static ifont * font1;
};

ifont *  tmp::font1 = OpenFont("MSMINCHO.TTF", 30, 2);

//dummy dummy dummy dummy dummy dummy



int mainHandler(int type, int par1, int par2) 
{


	static int i=0;
	fprintf(stderr, "event:  [%i %i %i]\n", type, par1, par2);

//	if (type == EVT_SHOW) 
	{

		ClearScreen();
//		FullUpdate();
		DrawRect(10, 18, 580, 104, 0);
		i++;
		{
			FillArea(12+i*36, 20, 36, 100, i*0x111111);
		}
//		FullUpdate();
		SetFont(tmp::font1, BLACK);
		char * msg="la-la-la русский 日本語";
		DrawString(50, 50, msg);
		DrawSymbol(50, 100, 62);


		FineUpdate();

	}

//	if (type == EVT_KEYPRESS) {
  // 	

	if (i>16) CloseApp();
  //
//	}

	return 0;


}


