#include "deck.hpp"
#include <string>
#include <iostream>
#ifdef WIN32
	
#else
	#include <dirent.h>
#endif

// ----------------- DeckInfo ------------------------

std::string DeckInfo::GetName()
{
	return name;
}

// ----------------- Deck ----------------------------

DeckInfoList Deck::getDeckList()
{
	DeckInfoList decks;

#ifdef WIN32
	
	decks.push_back(DeckInfo("test1"));

	decks.push_back(DeckInfo("test2"));
	decks.push_back(DeckInfo("test3"));
#else
	
	struct dirent **filelist;
	std::string temps;
	int n;
	n=scandir("./decks/",&filelist,0,alphasort);
	if (n<0)
	{
		std::cerr<<"no decks found"<<std::endl;
		//should we exit here?
	} else
	{
		for (int ii=0;ii<n;ii++)
		{
			temps=filelist[ii]->d_name;
			free (filelist[ii]);
			if  (temps.find(".anki")!=std::string::npos)
			decks.push_back(DeckInfo(temps));
			std::cout<<temps<<std::endl;
			//user *tu= user::load(temps);
			//pair<userlist::iterator,bool>p=users.insert(make_pair(temps,tu));
		}
		free(filelist);
		std::cout<<n<<" decks loaded"<<std::endl;
	}
#endif
	std::string sss;
	

		return decks;
}

Deck * Deck::loadDeck(DeckId id)
{
	Deck * newdeck = new Deck();
	return newdeck;
}


//------------------ QA - card field -----------------
std::string QA::ToString() const
{
	return std::string("QA stub value 日本語");
}

