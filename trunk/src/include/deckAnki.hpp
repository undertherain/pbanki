#include "deck.hpp"
#include <list>



class DeckAnki: public IDeck
{
private:
		std::list<Card> cardsDueBuffer;

public:
	Card GetNextCard();						//next card from queue
	void LoadStats();						//
	void LoadData();						//
	void Fetch();
};
