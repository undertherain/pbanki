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
	void ShowFront(const ICard & card);
	void ShowBack(const ICard & card);
	void run();
	
};
