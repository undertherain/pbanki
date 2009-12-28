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

#include "exceptions.hpp"
#include "view_pb.hpp"
#include "logger.hpp"

#define IDX_MENU_ABOUT 101
#define IDX_MENU_CLOSE 102
#define IDX_MENU_EXIT 103
#define IDX_MENU_STATS 104
#define IDX_MENU_SUSPEND 105

ViewPocketBook view;

class Globals
{
public:
	static ifont * fontCard;
	static ifont * fontFront;
	static ifont * fontBack;
	static ifont * fontButtons;
	static bool isConfigEditorActive;
	static char deckToLoadName[256];
	static bool isNewCardRequired;
};


ifont *  Globals::fontCard = NULL;
ifont *  Globals::fontFront = NULL;
ifont *  Globals::fontBack = NULL;
ifont *  Globals::fontButtons = NULL;
char Globals::deckToLoadName[256];
bool Globals::isConfigEditorActive=false;
bool Globals::isNewCardRequired=true;

int cindex=0;

void config_ok() {

	//	SaveConfig(testcfg);
	Logger logger;
	logger.WriteLog("We are in config handler\n");
	Globals::isConfigEditorActive=false;
	//really we should not be here
}

void itemChanged(char * item) {

	//	SaveConfig(testcfg);
	fprintf(stderr,"Selected item: %s \n",item);

	strncpy(Globals::deckToLoadName,item,256);

	Globals::isConfigEditorActive=false;
	SetEventHandler(mainHandler);
}


//-----------------------menu -------------------------------

static imenu menuMain[] = {

	{ ITEM_HEADER,   0, "Menu", NULL },
	{ ITEM_ACTIVE, IDX_MENU_SUSPEND, "Suspend card", NULL },
	{ ITEM_ACTIVE, IDX_MENU_CLOSE, "Close deck", NULL },
	{ ITEM_ACTIVE, IDX_MENU_STATS, "Statistics", NULL },
	{ ITEM_ACTIVE, IDX_MENU_ABOUT, "About", NULL },
	{ ITEM_SEPARATOR, 0, NULL, NULL },
	{ ITEM_ACTIVE, IDX_MENU_EXIT, "Exit", NULL },
	{ 0, 0, NULL, NULL }

};


void menu1_handler(int index) 
{
	switch (index) 
	{
		case IDX_MENU_CLOSE://close deck
			view.status=Status::stShowStatsFinal;
			mainHandler(0,0,0);
			break;

		case IDX_MENU_ABOUT:
			Message(ICON_INFORMATION, "Mindcraft", "Mindcraft v0.2.3\nWritten by Alexander Drozd",5000);
			break;

		case IDX_MENU_SUSPEND:
			view.model.SuspendCard();
			Globals::isNewCardRequired=true;
			view.status=Status::stShowFront;
			view.lastStatus=Status::stShowBack;
			mainHandler(0,0,0);
        		break;

		case IDX_MENU_STATS:
//			view.model.SuspendCard();
			view.status=Status::stShowStats;
//			view.lastStatus=Status::stShowBack;
			mainHandler(0,0,0);

			break;

		case IDX_MENU_EXIT:
			CloseApp();
			break;
		default:
			Message(ICON_INFORMATION, "warining", "Option is not yet supported\nOr internal errror",5000);
			break;
	}

}




int ViewPocketBook::SelectDeck()
{

	DeckInfoList decks = model.getDeckList();
	if (decks.size()==0)
	{
		//set status=noDeck
		status=Status::stNoDecks;
		return -1;
	} 
	else
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
	//	logger.WriteLog("Handle event");

	static int iter=0;
	static InkViewEvent lastEvent(0,0,0);
//	if (event.type)

	iter++;
	//	Logger.WriteLog("main handler %d\n",iter);
	//	fprintf(stderr, "\tevent:  [%i %i %i]\n", event.type, event.par1, event.par2);
	if (Globals::isConfigEditorActive)
	{
		logger.WriteLog("waiting for config to launch handler");
		return 0;
	}
	try
	{
		switch(event.type)
		{
		case EVT_INIT:
			logger.WriteLog("Doing INIT event");
			std::cout<<"sizeof int= "<<sizeof(int)<<std::endl;
			// occurs once at startup, only in main handler
			Globals::fontCard = OpenFont("YOzFontM.TTF", 40, 2);
			Globals::fontFront = OpenFont("YOzFontM.TTF", 50, 2);
			Globals::fontBack = OpenFont("YOzFontM.TTF", 40, 2);
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
		bool isMainLoopRepeatRequired;
		if ((event.type==EVT_KEYREPEAT) && (event.par1==KEY_UP))
		{
			//DrawTextRect(11, 11, 580, 500,"SHOW MENU!!!!!", ALIGN_LEFT | VALIGN_MIDDLE);
			//
			//SoftUpdate();
			OpenMenu(menuMain, cindex, 20, 20, menu1_handler);
			return 0;
		}
		do
		{
			isMainLoopRepeatRequired=false;
			switch (status)
			{
			case Status::stSelectDeck:
				logger.WriteLog("selecting deck\n");
				model.CloseDeck();

				if (!SelectDeck())
					status=Status::stLoadDeck;
				return 0;
				//		displayDeckList(model.getDeckList());
				//		status=Status::stLoadDeck;
				break;
			case Status::stNoDecks:
				HandleNoDecks(event);
				break;
			case Status::stLoadDeck:
				{
					DeckId id(Globals::deckToLoadName);
					Globals::isNewCardRequired=true;
					SetFont(Globals::fontCard, BLACK);
					ClearScreen();
					DrawString(50, 50, "Loading deck...");
					SoftUpdate();

					view.model.LoadDeck(id);
					view.model.LoadStats();
					std::cout<<"due cards: "<<model.GetNumCardsDueToday()<<std::endl;
					lastStatus=status;
					if (model.GetNumCardsDueToday()>0)
						status=Status::stShowFront;
					else
						status=Status::stNoMoreCards;		
					isMainLoopRepeatRequired=true;

				}
				break;
			case Status::stShowFront:
				//std::cout<<"show front" << std::endl;

				if (model.GetNumCardsDueToday()<1)
					status=Status::stNoMoreCards;		
				else
					HandleShowFront(event);
				break;
			case Status::stShowBack:
				HandleShowBack(event);
				break;
			case Status::stNoMoreCards:
				HandleNoMoreCards(event);
				break;
			case Status::stShowStats:
			case Status::stShowStatsFinal:
				HandleShowStats(event);
				break;
			case Status::stExit:
				CloseApp();
				break;
			default:
				//make error unexepected status
				std::cout<< "unexpexted status" <<std::endl;
				CloseApp();
				break;

			}
		} while(isMainLoopRepeatRequired);


	}
	catch(Exception ex)
	{
		std::cout<<"Error in vuewpb handle event: "<<ex.GetMessage()<<std::endl;
		ClearScreen();
		DrawTextRect(11, 11, 580, 500, const_cast<char *>((ex.GetMessage()+"\n press a key to exit").c_str()), ALIGN_LEFT | VALIGN_MIDDLE);
		SoftUpdate();
		status=Status::stExit;
	}



	//	if (type == EVT_KEYPRESS) {
	// 	

	//	if (iter>36) 
	//	{	
	//	logger.WriteLog("EXITING by iterations limit\n");
	//	CloseApp();
	//	}

	//
	//	}

	//	FullUpdate();
	//	SoftUpdate();
	lastEvent=event;
	return 0;
}

int ViewPocketBook::HandleShowFront(InkViewEvent event) 
{
	//std::cout<<"fetching new card" <<std::endl;
	if (Globals::isNewCardRequired)
	{
		lastStatus=status;
		card=model.getNextCard();
		Globals::isNewCardRequired=false;
	}
	//std::cout<<"drawing" <<std::endl;
//	fprintf(stderr,"ShowFront Handler\n");
//	fprintf(stderr, "\tevent:  [%i %i %i]\n", event.type, event.par1, event.par2);


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
	//	fprintf(stderr,"ShowBack Handler answer=%d \n",answerMark);
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
			if (answerMark>4) answerMark=4;
			break;
		case KEY_OK:
			if (answerMark==4) answerMark=-1;
			model.AnswerCard(answerMark);
			Globals::isNewCardRequired=true;
			if (model.GetNumCardsDueToday()>0)
				view.status=Status::stShowFront;
			else
				view.status=Status::stNoMoreCards;
	
			return 0;
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
		//	PartialUpdateBW(10,300,580,400);

		//drawing buttons
		SetFont(Globals::fontButtons, BLACK);
		DrawString(150, 723, "Again   Hard    Good    Easy    Suspend");
		DrawRect(140+answerMark*85, 720, 82, 30, 0);
		//FillArea(200+answerMark*40, 650, 36, 20, LGRAY);

		SoftUpdate();
	}
	return 0;
}
int ViewPocketBook::HandleNoDecks(InkViewEvent event) 
{
	SetFont(Globals::fontCard, BLACK);
//	DrawString(50, 50, "No decks found");
//	DrawString(50, 300, "Press ok to exit");
       	DrawTextRect(11, 11, 580, 300, "No decks found\nmake sure you have them in ./deck folder\nPress ok to exit", ALIGN_LEFT | VALIGN_MIDDLE);

	if ((event.type==EVT_KEYPRESS) && (event.par1==KEY_OK))
	{
		view.status=Status::stExit;
	}
	SoftUpdate();

	return 0;
}

int ViewPocketBook::HandleShowStats(InkViewEvent event) 
{
	SetFont(Globals::fontCard, BLACK);
	ClearScreen();
       	DrawTextRect(11, 11, 580, 300, const_cast<char *>(model.GetStatsStr().c_str()), ALIGN_LEFT | VALIGN_MIDDLE);

	if ((event.type==EVT_KEYPRESS) && (event.par1==KEY_OK))
	{
		if (view.status==Status::stShowStats)
			view.status=Status::stShowFront;
		else
		view.status=Status::stSelectDeck;
	}
	SoftUpdate();

	return 0;
}

int ViewPocketBook::HandleNoMoreCards(InkViewEvent event) 
{
	static int buttonIndex=0;
	ClearScreen();
	SetFont(Globals::fontCard, BLACK);
	
	DrawTextRect(11, 11, 580, 500,const_cast<char *>(model.GetStatsForTomorrowStr().c_str()), ALIGN_LEFT | VALIGN_TOP);


	if (event.type==EVT_KEYPRESS) 
		switch(event.par1)
	{
		case KEY_UP:
			buttonIndex--;
			if (buttonIndex<0) buttonIndex=0;
			break;
		case KEY_DOWN:
			buttonIndex++;
			if (buttonIndex>2) buttonIndex=2;
			break;
		case KEY_OK:
			switch (buttonIndex)
			{
				case 0:
					model.LearnMore();
					view.status=Status::stShowFront;
					break;
				case 1:
					model.CloseDeck();
					view.status=Status::stSelectDeck;
					break;
				case 2:
					view.status=Status::stExit;
        				break;
			}
			return 0;
			break;
		default:
			//do nothing
			break;
	}
       	DrawTextRect(200, 300, 200, 40,"Learn more", ALIGN_CENTER | VALIGN_MIDDLE);
       	DrawTextRect(200, 350, 200, 40,"Close deck", ALIGN_CENTER | VALIGN_MIDDLE);
       	DrawTextRect(200, 400, 200, 40,"Exit", ALIGN_CENTER | VALIGN_MIDDLE);
	DrawRect(200, 303 + buttonIndex*50, 200, 35, 0);

	SoftUpdate();

	return 0;
}

void ViewPocketBook::ShowFront()
{
	std::cout<<"we are in pb, show front:"<<card.front.ToString()<<std::endl;
//	for (int i=0;card.front.ToString().c_str()[i]!=0;i++)
//	{
//		std::cout<<static_cast<int>((unsigned char)card.front.ToString().c_str()[i])<<std::endl;
//	}
	DrawRect(10, 10, 580, 300, 0);
	SetFont(Globals::fontFront, BLACK);
//	DrawString(50, 50, card.front.ToString().c_str());
//	DrawTextRect(11, 11, 580, 300,"damn", ALIGN_CENTER | VALIGN_MIDDLE);
	DrawTextRect(11, 11, 580, 300, const_cast<char *>(card.front.ToString().c_str()), ALIGN_CENTER | VALIGN_MIDDLE);
//	std::cout<<"done" <<std::endl;
//	return;
	
	//DrawTextRect(11, 11, 580, 300, "can't \"render this card", ALIGN_CENTER | VALIGN_MIDDLE);
	
	ShowStatusBar();
}

void ViewPocketBook::ShowBack()
{
	DrawRect(10, 315, 580, 390, 0);
	SetFont(Globals::fontBack, BLACK);
	//DrawString(50, 350, card.back.ToString().c_str());
	DrawTextRect(11, 316, 580, 390, const_cast<char *>(card.back.ToString().c_str()), ALIGN_CENTER | VALIGN_MIDDLE);
}

void ViewPocketBook::ShowStatusBar()
{
	DrawRect(10, 760, 580, 30, 0);
	SetFont(Globals::fontButtons, BLACK);
	DrawTextRect(11, 760, 580, 30, const_cast<char *>(model.GetStatus().c_str()), ALIGN_CENTER | VALIGN_MIDDLE);
}

int mainHandler(int type, int par1, int par2) 
{

	view.HandleEvent(InkViewEvent(type, par1, par2));
	return 0;


}


