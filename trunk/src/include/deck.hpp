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
public:
	DeckInfo(std::string _name):name(_name)	
	{}
	//DeckInfo(const DeckInfo & deck):name(deck.name)
	//{}

	std::string GetName();
};

//question or answer
class QA	
{

};

class Card
{
public:
	QA front;
	QA back;
};
