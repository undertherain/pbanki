#include <string>
#include <list>

class DeckInfo;

typedef std::list<DeckInfo> DeckInfoList ;
typedef std::string DeckId ;


class Deck  //contains cards
{
public:
	static DeckInfoList getDeckList();
	static Deck * loadDeck(DeckId id);

};

class DeckInfo //light-weighted class for list of available decks
{
public:
	std::string name;
	DeckInfo(std::string _name):name(_name)
	{}
};

class QA	//question or answer
{

};

class Card
{
public:
	QA front;
	QA back;

};
