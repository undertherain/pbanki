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
#include "logger.hpp"


ViewPocketBook view;

class Globals
{
public:
	static ifont * fontCard;
	static ifont * fontButtons;
	static bool isConfigEditorActive;
};

ifont *  Globals::fontCard = NULL;
ifont *  Globals::fontButtons = NULL;

bool Globals::isConfigEditorActive=false;


void config_ok() {

	//	SaveConfig(testcfg);
	Logger logger;
	logger.WriteLog("We are in config handler\n");
	Globals::isConfigEditorActive=false;
	//really we should not be here
}

void itemChanged(char * item) {

	//	SaveConfig(testcfg);
	fprintf(stderr,"We are in item %s \n",item);
	DeckId id(item);

	SetFont(Globals::fontCard, BLACK);
	ClearScreen();
	DrawString(50, 50, "Loading deck...");
	SoftUpdate();

	view.model.LoadDeck(id);
	view.model.LoadStats();
	SetEventHandler(mainHandler);
	Globals::isConfigEditorActive=false;

}



int ViewPocketBook::SelectDeck()
	{
		DeckInfoList decks = model.getDeckList();
		if (decks.size()==0)
		{
			//set status=noDeck
			status=Status::stNoDecks;
			return -1;
		} else
		{
			std::cout<<"starting deck selector"<<std::endl;
			iconfigedit * decksConfigEdit = new iconfigedit[decks.size()+1];
			int cnt=0;
			for (DeckInfoList::iterator i=decks.begin();i!=decks.end();i++)
			{	
				iconfigedit newConfigLine;
				newConfigLine.type = CFG_INFO;
				newConfigLine.icon = NULL;
				newConfigLine.text = new char[i->GetName().length()];
				strcpy(newConfigLine.text,i->GetName().c_str());
				newConfigLine.hint = "new:? due:?";

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
			Globals::isConfigEditorActive=true;
			OpenConfigEditor("Select Deck", cfgDeckList, decksConfigEdit, config_ok,itemChanged);
			return 0;
		}
	}


int ViewPocketBook::HandleEvent(InkViewEvent event) 
{
	Logger logger;
	logger.WriteLog("Handle event");

	static int iter=0;

	iter++;
//	Logger.WriteLog("main handler %d\n",iter);
	fprintf(stderr, "\tevent:  [%i %i %i]\n", event.type, event.par1, event.par2);
        if (Globals::isConfigEditorActive)
	{
		logger.WriteLog("waiting for config to launch handler");
		return 0;
	}
	switch(event.type)
	{
			case EVT_INIT:
				logger.WriteLog("Doing INIT event");

				// occurs once at startup, only in main handler
				Globals::fontCard = OpenFont("YOzFontM.TTF", 30, 2);
				Globals::fontButtons = OpenFont("LiberationMono.ttf", 18, 2);

				ClearScreen();
				DrawString(160, 253, "Starting Anki");
	
				FullUpdate();

				//return 0;
				break;

			case EVT_EXIT:
				logger.WriteLog("EXIT event recieved");
				status=Status::stExit;
				break;
			default:

				break;
				//hmm.... 

	}

	switch (status)
	{
	case Status::stSelectDeck:
	logger.WriteLog("selecting deck\n");
		
		if (!SelectDeck())
			status=Status::stShowFront;
		return 0;
//		displayDeckList(model.getDeckList());
//		status=Status::stLoadDeck;
		break;
	case Status::stNoDecks:
		HandleNoDecks(event);
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
		logger.WriteLog("EXITING by iterations limit\n");
		CloseApp();
	}
	//
	//	}

	//	FullUpdate();
	//	SoftUpdate();
	return 0;
}

int ViewPocketBook::HandleShowFront(InkViewEvent event) 
{	if (lastStatus!=status)
	{
		lastStatus=status;
		card=model.getNextCard();
	}
	fprintf(stderr,"ShowFront Handler\n");
	fprintf(stderr, "\tevent:  [%i %i %i]\n", event.type, event.par1, event.par2);
	
	
		ClearScreen();
		//	FullUpdate();
		ShowFront();
		SoftUpdate();
	
	if ((event.type==EVT_KEYPRESS) && (event.par1==KEY_OK))
	{
		view.status=Status::stShowBack;
	}
			
	return 0;
}
int ViewPocketBook::HandleShowBack(InkViewEvent event) 
{
	static int answerMark=0;
	lastStatus=status;
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
			case KEY_OK:
			model.AnswerCard(answerMark);
			view.status=Status::stShowFront;
			break;
		default:
			//do nothing
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

		SoftUpdate();
	}
	return 0;
}
int ViewPocketBook::HandleNoDecks(InkViewEvent event) 
{
	SetFont(Globals::fontCard, BLACK);
	DrawString(50, 50, "No decks found");
	DrawString(50, 300, "Press ok to exit");

	if ((event.type==EVT_KEYPRESS) && (event.par1==KEY_OK))
	{
		view.status=Status::stExit;
	}
	SoftUpdate();
			
	return 0;
}

void ViewPocketBook::ShowFront()
{
		DrawRect(10, 10, 580, 300, 0);
		SetFont(Globals::fontCard, BLACK);
		//DrawString(50, 50, card.front.ToString().c_str());
		DrawTextRect(11, 11, 580, 300, const_cast<char *>(card.front.ToString().c_str()), ALIGN_CENTER | VALIGN_MIDDLE);
		ShowStatusBar();
}

void ViewPocketBook::ShowBack()
{
		DrawRect(10, 315, 580, 300, 0);
		SetFont(Globals::fontCard, BLACK);
		//DrawString(50, 350, card.back.ToString().c_str());
		DrawTextRect(11, 316, 580, 300, const_cast<char *>(card.back.ToString().c_str()), ALIGN_CENTER | VALIGN_MIDDLE);
}

void ViewPocketBook::ShowStatusBar()
{
		DrawRect(10, 755, 580, 30, 0);
		SetFont(Globals::fontButtons, BLACK);
		DrawTextRect(11, 755, 580, 30, const_cast<char *>(model.GetStatus().c_str()), ALIGN_CENTER | VALIGN_MIDDLE);
}

int mainHandler(int type, int par1, int par2) 
{

	view.HandleEvent(InkViewEvent(type, par1, par2));
	return 0;


}


