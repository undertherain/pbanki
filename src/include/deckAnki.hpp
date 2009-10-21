#include "deck.hpp"
#include <list>


class DeckAnki: public IDeck
{
private:
		std::list<ICard> cardsDueBuffer;
		ICard * lastCard;

public:
	ICard GetNextCard();						//next card from queue
	void LoadStats();						//
	void LoadData();						//
	void AnswerCard(Answer answer);
	void Fetch();
};
