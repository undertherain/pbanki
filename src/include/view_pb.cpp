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


ViewPocketBook view;

class Globals
{
public:
	static ifont * fontCard;
	static ifont * fontButtons;
};

ifont *  Globals::fontCard = OpenFont("MSMINCHO.TTF", 30, 2);
ifont *  Globals::fontButtons = OpenFont("LiberationMono.ttf", 18, 2);


void config_ok() {

	//	SaveConfig(testcfg);
	fprintf(stderr,"We are in config handler\n");
	//really we should not be here
}

void itemChanged(char * item) {

	//	SaveConfig(testcfg);
	fprintf(stderr,"We are in item %s \n",item);
	DeckId id(item);
	view.model.loadDeck(id);
	SetEventHandler(mainHandler);

}

//dummy dummy dummy dummy dummy dummy


void ViewPocketBook::SelectDeck()
	{
		DeckInfoList decks = model.getDeckList();
		iconfigedit * decksConfigEdit = new iconfigedit[decks.size()+1];
		int cnt=0;
		for (DeckInfoList::iterator i=decks.begin();i!=decks.end();i++)
		{	
			iconfigedit newConfigLine;
			newConfigLine.type = CFG_INFO;
			newConfigLine.icon = NULL;
			newConfigLine.text = new char[i->GetName().length()];
			strcpy(newConfigLine.text,i->GetName().c_str());
			newConfigLine.hint = "new:5 due:10";

			newConfigLine.name = new char[i->GetName().length()];
			strcpy(newConfigLine.name,i->GetName().c_str());

			newConfigLine.deflt = NULL;
			newConfigLine.variants = NULL;
			newConfigLine.submenu = NULL;

			decksConfigEdit[cnt++]=newConfigLine;
			//std::cout<<i->GetName()+" \n";	
		}
		iconfigedit emptyConfigLine={ 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
		decksConfigEdit[cnt]=emptyConfigLine;
		iconfig *cfgDeckList = OpenConfig("/mnt/ext1/test.cfg", NULL);
		//get decks list
		//fill array of 
		OpenConfigEditor("Select Deck", cfgDeckList, decksConfigEdit, config_ok,itemChanged);
	}


int ViewPocketBook::HandleEvent(InkViewEvent event) 
{
	static int iter=0;
	if (event.type == EVT_INIT) {
		// occurs once at startup, only in main handler
		//		return 0;
	}

	iter++;
	fprintf(stderr,"main handler %d\n",iter);
	fprintf(stderr, "\tevent:  [%i %i %i]\n", event.type, event.par1, event.par2);
	
	switch(event.type)
	{
			case EVT_INIT:
				return 0;
						break;

			default:
				break;
				//hmm.... 

	}

	switch (status)
	{
	case Status::stSelectDeck:
	fprintf(stderr,"selecting deck\n");
		
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
		HandleShowBack(event);
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

	if (iter>36) 
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
	fprintf(stderr,"ShowFront Handler\n");
	fprintf(stderr, "\tevent:  [%i %i %i]\n", event.type, event.par1, event.par2);
	

		ClearScreen();
		//	FullUpdate();
		ShowFront();
		FineUpdate();
	
	if ((event.type==EVT_KEYPRESS) && (event.par1==KEY_OK))
	{
		view.status=Status::stShowBack;
	}
			
	return 0;
}
int ViewPocketBook::HandleShowBack(InkViewEvent event) 
{
	static int answerMark=0;
	
	fprintf(stderr,"ShowBack Handler answer=%d \n",answerMark);
	//fprintf(stderr, "event:  [%i %i %i]\n", event.type, event.par1, event.par2);
	if (event.type==EVT_KEYPRESS) 
		switch(event.par1)
		{
			case KEY_LEFT:
				answerMark--;
				if (answerMark<0) answerMark=0;
			break;
			case KEY_RIGHT:
				answerMark++;
				if (answerMark>3) answerMark=3;
			break;
		default:
			//next card
			view.status=Status::stShowFront;
			break;
		}

	//	if (type == EVT_SHOW) 
	{

		ClearScreen();
		ShowFront();
		ShowBack();
		//drawing buttons
		SetFont(Globals::fontButtons, BLACK);
		DrawString(160, 653, "Again   Hard    Good    Easy");
		DrawRect(150+answerMark*85, 650, 82, 30, 0);
		//FillArea(200+answerMark*40, 650, 36, 20, LGRAY);

		FineUpdate();
	}
	return 0;
}
void ViewPocketBook::ShowFront()
{
		DrawRect(10, 10, 580, 300, 0);
		SetFont(Globals::fontCard, BLACK);
		DrawString(50, 50, card.front.ToString().c_str());
}
void ViewPocketBook::ShowBack()
{
		DrawRect(10, 315, 580, 300, 0);
		SetFont(Globals::fontCard, BLACK);
		DrawString(50, 350, card.back.ToString().c_str());
}

int mainHandler(int type, int par1, int par2) 
{

	view.HandleEvent(InkViewEvent(type, par1, par2));
	return 0;


}


