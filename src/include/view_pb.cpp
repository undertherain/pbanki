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
	ViewPocketBook():status(Status::stSelectDeck){};
	Status::enumStatuses status;
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
	int HandleShowFront(InkViewEvent event);
	int HandleEvent(InkViewEvent event);

};

int ViewPocketBook::HandleEvent(InkViewEvent event) 
{
	static int iter=0;
	static int i=0;
	if (event.type == EVT_INIT) {
		// occurs once at startup, only in main handler
		//		return 0;
	}

	iter++;
	fprintf(stderr,"main handler %d\n",iter);
	fprintf(stderr, "event:  [%i %i %i]\n", event.type, event.par1, event.par2);
	

	switch (status)
	{
	case Status::stSelectDeck:
		SelectDeck();
		status=Status::stShowFront;
		return 0;
//		displayDeckList(model.getDeckList());
//		status=Status::stLoadDeck;
		break;
	case Status::stLoadDeck:
		{
		//	DeckId id;
		//	id=selectDeck();
		//	if (id.length()==0)
		//	{
		//		status=Status::stExit;	
		//	}
		//	else
		//	{
		//		model.loadDeck(id);
		//		status=Status::stShowFront;
		//	}
		}
		break;
	case Status::stShowFront:
		HandleShowFront(event);
		break;
	case Status::stShowBack:
		{
//			ShowBack(card);
		}
		break;
	case Status::stExit:
		CloseApp();
		break;
	default:
		//make error unexepected status
		CloseApp();
		break;

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

int ViewPocketBook::HandleShowFront(InkViewEvent event) 
{
	static int i=0;
	i++;
	fprintf(stderr,"ShowFront Handler\n");
	fprintf(stderr, "event:  [%i %i %i]\n", event.type, event.par1, event.par2);
	//	if (type == EVT_SHOW) 
	{

		ClearScreen();
		//	FullUpdate();
		DrawRect(10, 18, 580, 104, 0);

		{
			FillArea(12+i*36, 20, 36, 100, i*0x111111);
		}
		//		FullUpdate();
		SetFont(tmp::font1, BLACK);
		char * msg="front русский 日本語";
		DrawString(50, 50, msg);
		DrawSymbol(50, 100, 62);


		FineUpdate();

	}
	return 0;
}

int mainHandler(int type, int par1, int par2) 
{
	static ViewPocketBook view;
	view.HandleEvent(InkViewEvent(type, par1, par2));
	return 0;


}


