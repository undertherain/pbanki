#include "model.hpp"
#include "controller.hpp"
#include "string"


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
	void ShowFront(const Card & card);
	void ShowBack(const Card & card);
	void run();
	
};
