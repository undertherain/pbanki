#include <string>
#include <list>

class DeckInfo;

typedef std::list<DeckInfo> DeckInfoList ;
typedef std::string DeckId ;


//contains cards
class Deck  
{
public:
	static DeckInfoList getDeckList();
	static Deck * loadDeck(DeckId id);

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

	std::string GetName();
	int getNumCardsTotal();
};

//question or answer
class QA	
{
public:
	std::string ToString() const;
};

class Card
{
public:
	QA front;
	QA back;
};
