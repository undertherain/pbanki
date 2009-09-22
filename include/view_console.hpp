#include "model.hpp"
#include "string"

class ViewConsole
{
private:
	int status;
	Model model;
public:
	ViewConsole():status(1){};
	void displayDeckList(DeckInfoList);
	DeckId selectDeck();
	void refresh();
	void ShowFront(Card & card);
	void run();
	
};
