
#ifndef MINDCRAFT_VIEW_PB
#define MINDCRAFT_VIEW_PB


#include "model.hpp"
#include "controller.hpp"


struct InkViewEvent
{
	int type;
	int par1;
	int par2;
	InkViewEvent(int _type, int _par1, int _par2):type(_type),par1(_par1),par2(_par2){};
};

int mainHandler(int type, int par1, int par2);

// private members
class ViewPocketBook
{
private:


public:
	ICard  card;
	Model model;
	ViewPocketBook():card(CardField("Error"),CardField("Error")),status(Status::stSelectDeck){};
	Status::enumStatuses status;
	Status::enumStatuses lastStatus;
	int SelectDeck();
	
	int HandleNoDecks(InkViewEvent event);
	int HandleShowFront(InkViewEvent event);
	int HandleShowBack(InkViewEvent event);
	int HandleNoMoreCards(InkViewEvent event);
	int HandleEvent(InkViewEvent event);
	int HandleShowStats(InkViewEvent event);
	void ShowFront();
	void ShowBack();
	void ShowStatusBar();

};



#endif // MINDCRAFT_VIEW_PB
