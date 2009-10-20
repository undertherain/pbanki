// deckAnki
// Copyright (c) 2009 Alexander A. Drozd 

#include "deckAnki.hpp"
#include "libs/sqlite3.h"
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <ctime>

#define DECK_BUFFER_SIZE 100;

typedef std::map<std::string,std::string> StringMap;
// ------------------------- SQLite routines ------------------------
class SQLiteResults
{
public:
	static std::vector<StringMap> values;

};
std::vector<StringMap> SQLiteResults::values;

static int callback(void *NotUsed, int argc, char **argv, char **azColName)    //who does free for these poiners???????
{
	int i;
	//printf("NotUsed = %d\n", NotUsed);
	StringMap newMap;
	for(i=0; i<argc; i++){
		newMap.insert(std::pair<std::string,std::string>( azColName[i], argv[i] ? argv[i] : "NULL") );
	//	printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	SQLiteResults::values.push_back(newMap);
	printf("\n");
	return 0;
}
// format helper
class FormatHelper
{
public:
	static std::string GetTimeStr()
	{
		time_t seconds;
		seconds = time (NULL);
		std::ostringstream out;
		out << seconds;
		std::string str = out.str();
		return str;

	}
};

//--------------------------- Card Queue (buffer) -------------------
void DeckAnki::Fetch()
{
	//start fetching failed cards until we reach buffer limit
	sqlite3 *dbDeck;
	char *zErrMsg = NULL;
	int retCode;

	//open sqlite
	//select random card

	retCode = sqlite3_open_v2(fileName.c_str(), &dbDeck,SQLITE_OPEN_READWRITE,NULL);

	if( retCode )
	{
		std::cout<< "getNextCard. Can't open database: " <<  sqlite3_errmsg(dbDeck)<<std::endl;
		sqlite3_close(dbDeck);
		
		return ;
		//throw exception here?
	}
	std::cout<< "reading sqlite  "<< fileName <<" " <<retCode<<std::endl;
	std::string query="SELECT * FROM cards where type=0 ORDER BY combinedDue LIMIT 1";
	SQLiteResults::values.clear();
	retCode = sqlite3_exec(dbDeck, query.c_str(), callback, 0, &zErrMsg);

	std::cout<<"size = "<< SQLiteResults::values.size()<<std::endl;
	if( retCode!=SQLITE_OK )
	{
		std::cout << "SQL error: " <<  zErrMsg << std::endl;
		sqlite3_free(zErrMsg);
	}
	//close sqlite
	sqlite3_close(dbDeck);

	StringMap row = SQLiteResults::values[0];
	std::string strFront = row["question"];
	std::string strBack = row["answer"];
	CardField fldFront(strFront);
	CardField fldBack(strBack);
	//Card card(CardField("stub front 日本語"),CardField("stub back 日本語"));
	Card card(fldFront,fldBack);
	cardsDueBuffer.push_back(card);
	//if no mo failed cards - start loading review cards, until we reach biffer limit
	//if no more review cards - load new cards
}

//--------------------------- DeckAnki ------------------------------
Card DeckAnki::GetNextCard()
{

	if (!cardsDueBuffer.empty())
	{
		Card card=cardsDueBuffer.front();
		cardsDueBuffer.pop_front();
		return card;
	}

	Card cardError(CardField("No more cards "),CardField("No more cards "));
	return cardError;
}

//--------------------------- DeckAnki ------------------------------
void DeckAnki::LoadStats()
{
	sqlite3 *dbDeck;
	char *zErrMsg = NULL;
	int retCode;

	//open sqlite
	//select random card
	std::cout<< "Loading deck stats " <<std::endl;

	retCode = sqlite3_open_v2(fileName.c_str(), &dbDeck,SQLITE_OPEN_READWRITE,NULL);

	if( retCode )
	{
		std::cout<< "DeckAnki:LoadStats. can't open database: " <<  sqlite3_errmsg(dbDeck)<<std::endl;
		sqlite3_close(dbDeck);

		return ;
		//throw exception here?
	}
	//new cards total
	std::string query="select count(id) from cards where type = 2 and priority in (1,2,3,4)";
	SQLiteResults::values.clear();
	retCode = sqlite3_exec(dbDeck, query.c_str(), callback, 0, &zErrMsg);
	if( retCode!=SQLITE_OK )
	{
		std::cout << "SQL error: " <<  zErrMsg << std::endl;
		sqlite3_free(zErrMsg);
	}
	{
		std::istringstream tempStream(SQLiteResults::values[0]["count(id)"]);
		tempStream>>numCardsNewTotal;
	}
	std::cout<<"NEW cards total = "<< numCardsNewTotal <<std::endl;
	//cards due today
	query="select count(id) from cards where combinedDue < " + FormatHelper::GetTimeStr() + " and priority in (1,2,3,4) and type in (0, 1)"; 
	SQLiteResults::values.clear();
	retCode = sqlite3_exec(dbDeck, query.c_str(), callback, 0, &zErrMsg);
	if( retCode!=SQLITE_OK )
	{
		std::cout << "SQL error: " <<  zErrMsg << std::endl;
		sqlite3_free(zErrMsg);
	}
	{
		std::istringstream tempStream(SQLiteResults::values[0]["count(id)"]);
		tempStream>>numCardsNewTotal;
	}
	std::cout<<"DUE cards today = "<< numCardsNewTotal <<std::endl;


	//close sqlite
	sqlite3_close(dbDeck);
}

void DeckAnki::LoadData()
{
	std::cout<<"Loading deck data " <<std::endl;
	Fetch();
}