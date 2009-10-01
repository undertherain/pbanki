#include "model.hpp"
#include "string"

class Status
{
public:
	 enum enumStatuses 	{stExit=0,stSelectDeck,stLoadDeck,stShowFront,stShowBack};
};

class ViewConsole
{
private:
	Status::enumStatuses status;
	Model model;
public:
	ViewConsole():status(Status::stSelectDeck){};
	void displayDeckList(DeckInfoList);
	DeckId selectDeck();
	void refresh();
	void ShowFront(Card & card);
	void run();
	
};
