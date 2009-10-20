#ifndef MINDCRAFT_DECK
#define MINDCRAFT_DECK

#include <string>
#include <list>

class DeckInfo;
class Card;

typedef std::list<DeckInfo> DeckInfoList ;
typedef std::string DeckId ;


//contains cards
class IDeck  
{
protected:
	std::string fileName;
public:
	static DeckInfoList getDeckList();
	static IDeck * loadDeck(DeckId id);
	virtual Card getNextCard()=0;						//next card from queue
	virtual ~IDeck(){}

};

//light-weighted class for list of available decks
class DeckInfo 
{
private:
	std::string name;
	int numCardsDueToday;
	int numCardsNewToday;
	int numCardsTotal;

public:

	DeckInfo(std::string fileName);
	
	//DeckInfo(const DeckInfo & deck):name(deck.name)
	//{}

	std::string GetName() const;
	int getNumCardsTotal() const;
};

//question or answer
class CardField	
{
private:
	std::string innerText;
public:
	std::string ToString() const;
	CardField(std::string _innerText):innerText(_innerText){};
};

class Card
{
public:
	CardField front;
	CardField back;
	Card(CardField _front,CardField _back):front(_front),back(_back){};
};


#endif // MINDCRAFT_CONTROLLER

