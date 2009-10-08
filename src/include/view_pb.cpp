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


extern const ibitmap item1, item2;

class tmp
{
public:
	static ifont * font1;
};

ifont *  tmp::font1 = OpenFont("MSMINCHO.TTF", 30, 2);


static iconfigedit testce2[] = {

	{ CFG_INFO, NULL, "Deck Name", "Due: 40, New: 10", NULL, NULL, NULL, NULL },
	{ CFG_INFO, NULL, "About device", "Information about electronic book", NULL, NULL, NULL, NULL },
	{ CFG_INFO, NULL, "About device", "Information about electronic book", NULL, NULL, NULL, NULL },
	{ CFG_INFO, NULL, "About device", "Information about electronic book", NULL, NULL, NULL, NULL },
	{ CFG_INFO, NULL, "About device", "Information about electronic book", NULL, NULL, NULL, NULL },
	{ CFG_INFO, NULL, "About device", "Information about electronic book", NULL, NULL, NULL, NULL },
	{ CFG_INFO, NULL, "About device", "Information about electronic book", NULL, NULL, NULL, NULL },
	{ CFG_INFO, NULL, "About device", "Information about electronic book", NULL, NULL, NULL, NULL },
	{ CFG_INFO, NULL, "About device", "Information about electronic book", NULL, NULL, NULL, NULL },
	{ CFG_INFO, NULL, "About device", "Information about electronic book", NULL, NULL, NULL, NULL },
	{ CFG_INFO, NULL, "About device", "Information about electronic book", NULL, NULL, NULL, NULL },
	{ CFG_INFO, NULL, "About device", "Information about electronic book", NULL, NULL, NULL, NULL },
	{ CFG_INFO, NULL, "About device", "Information about electronic book", NULL, NULL, NULL, NULL },
	{ CFG_INFO, NULL, "About device", "Information about electronic book3", NULL, NULL, NULL, NULL },
	{ CFG_INFO, NULL, "About device", "Information about electronic book4", NULL, NULL, NULL, NULL },
	{ CFG_INFO, NULL, "About device", "Information about electronic book5", NULL, NULL, NULL, NULL },
	{ CFG_INFO, NULL, "About device", "Information about electronic book6", NULL, NULL, NULL, NULL },
	{ CFG_INFO, NULL, "About device", "Information about electronic book7", NULL, NULL, NULL, NULL },
	{ CFG_INFO, NULL, "About device", "Information about electronic book8", NULL, NULL, NULL, NULL },
	{ CFG_TEXT, NULL, "Text edit", "Example of text edit control", "param.textedit", "qwerty", NULL, NULL },
	{ 0, NULL, NULL, NULL, NULL, NULL, NULL}

};


void config_ok() {

//	SaveConfig(testcfg);

}

//dummy dummy dummy dummy dummy dummy
class ViewPocketBook
{
public:
	void SelectDeck()
	{
		iconfig *testcfg = OpenConfig("/mnt/ext1/test.cfg", NULL);
//		testce2[] =	{			
//			{ CFG_INFO, &item1, "About device", "Information about electronic book", NULL, NULL, NULL, NULL },
//			{ CFG_TEXT, &item2, "Text edit", "Example of text edit control", "param.textedit", "qwerty", NULL, NULL },
//			{ 0, NULL, NULL, NULL, NULL, NULL, NULL}		
//		};	

		OpenConfigEditor("Select Deck", testcfg, testce2, config_ok, NULL);
	}
};

int newHandler(int type, int par1, int par2) 
{
	fprintf(stderr,"new handler\n");
	fprintf(stderr, "event:  [%i %i %i]\n", type, par1, par2);

}

int mainHandler(int type, int par1, int par2) 
{
	static ViewPocketBook view;
	static int iter=0;
	static int i=0;
	if (type == EVT_INIT) {
		// occurs once at startup, only in main handler
//		return 0;
	}
	SetEventHandler(newHandler);
	iter++;
	fprintf(stderr,"main handler %d\n",iter);
	fprintf(stderr, "event:  [%i %i %i]\n", type, par1, par2);
	if (iter==1)
	{
		view.SelectDeck();
	}
	
//	if (type == EVT_SHOW) 
	{

		ClearScreen();
	//	FullUpdate();
		DrawRect(10, 18, 580, 104, 0);
		
		{
			FillArea(12+iter*36, 20, 36, 100, i*0x111111);
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

	if (iter>16) 
	{	
		printf("EXITING\n");
		CloseApp();
	}
  //
//	}

	return 0;


}


