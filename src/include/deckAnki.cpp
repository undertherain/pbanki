// deckAnki
// Copyright (c) 2009 Alexander A. Drozd 

#include "deckAnki.hpp"
#include "libs/sqlite3.h"
#include "utils.hpp"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

#define DECK_BUFFER_SIZE 3
#define MAX_NEW_CARDS 10

typedef std::map<std::string,std::string> StringMap;
// ------------------------- SQLite routines ------------------------
static int callback(void *NotUsed, int argc, char **argv, char **azColName);

class SQLiteHelper //should be changed with database class
{
public:
	static std::vector<StringMap> values;

};
std::vector<StringMap> SQLiteHelper::values;
static int callback(void *NotUsed, int argc, char **argv, char **azColName)    //who does free for these poiners???????
{
	int i;
	//printf("NotUsed = %d\n", NotUsed);
	StringMap newMap;
	for(i=0; i<argc; i++){
		newMap.insert(std::pair<std::string,std::string>( azColName[i], argv[i] ? argv[i] : "NULL") );
	//	printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	SQLiteHelper::values.push_back(newMap);
	printf("\n");
	return 0;
}


//--------------------------- Fetching Card Queue (buffer) -------------------
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
	std::string query="SELECT * FROM cards where type=0 ORDER BY combinedDue LIMIT " + FormatHelper::ConvertToStr(DECK_BUFFER_SIZE);
	
	SQLiteHelper::values.clear();
	retCode = sqlite3_exec(dbDeck, query.c_str(), callback, 0, &zErrMsg);

	std::cout<<"size = " << SQLiteHelper::values.size() << std::endl;
	if( retCode!=SQLITE_OK )
	{
		std::cout << "SQL error: " <<  zErrMsg << std::endl;
		sqlite3_free(zErrMsg);
	}
	//close sqlite
	sqlite3_close(dbDeck);

	for (unsigned int i=0;i<SQLiteHelper::values.size();i++)
	{
		StringMap row = SQLiteHelper::values[i];
		std::string strFront = row["question"];
		std::string strBack = row["answer"];
		CardField fldFront(strFront);
		CardField fldBack(strBack);
		ICard card(fldFront,fldBack);
		cardsDueBuffer.push_back(card);
	}
	//if no mo failed cards - start loading review cards, until we reach biffer limit
	//if no more review cards - load new cards
}

//--------------------------- Next Card ------------------------------
ICard DeckAnki::GetNextCard()
{

	if (!cardsDueBuffer.empty())
	{
		ICard card=cardsDueBuffer.front();
		cardsDueBuffer.pop_front();
		if (lastCard!=NULL)
		{
			delete lastCard;
		}
		lastCard = new ICard(card);
		return card;
	}

	ICard cardError(CardField("No more cards "),CardField("No more cards "));
	return cardError;
}

//--------------------------- Load Stats ------------------------------
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
	SQLiteHelper::values.clear();
	retCode = sqlite3_exec(dbDeck, query.c_str(), callback, 0, &zErrMsg);
	if( retCode!=SQLITE_OK )
	{
		std::cout << "SQL error: " <<  zErrMsg << std::endl;
		sqlite3_free(zErrMsg);
	}
	numCardsNewTotal=FormatHelper::StrToInt(SQLiteHelper::values[0]["count(id)"]);
	std::cout<<"NEW cards total = "<< numCardsNewTotal <<std::endl;

	//cards new today
	numCardsNewToday=std::min(numCardsNewTotal,MAX_NEW_CARDS);
	std::cout<<"NEW cards today = "<< numCardsNewToday <<std::endl;

	//cards due today
	query="select count(id) from cards where combinedDue < " + FormatHelper::GetTimeStr() + " and priority in (1,2,3,4) and type in (0, 1)"; 
	SQLiteHelper::values.clear();
	retCode = sqlite3_exec(dbDeck, query.c_str(), callback, 0, &zErrMsg);
	if( retCode!=SQLITE_OK )
	{
		std::cout << "SQL error: " <<  zErrMsg << std::endl;
		sqlite3_free(zErrMsg);
	}
	numCardsNewTotal=FormatHelper::StrToInt(SQLiteHelper::values[0]["count(id)"]);
	std::cout<<"DUE cards today = "<< numCardsNewTotal <<std::endl;


	//cards failed today
	query="select count(id) from cards where combinedDue < " + FormatHelper::GetTimeStr() + " and priority in (1,2,3,4) and type = 0"; 
	SQLiteHelper::values.clear();
	retCode = sqlite3_exec(dbDeck, query.c_str(), callback, 0, &zErrMsg);
	if( retCode!=SQLITE_OK )
	{
		std::cout << "SQL error: " <<  zErrMsg << std::endl;
		sqlite3_free(zErrMsg);
	}
	numCardsFailedToday=FormatHelper::StrToInt(SQLiteHelper::values[0]["count(id)"]);
	std::cout<<"FAILED cards today = "<< numCardsFailedToday <<std::endl;

	//cards revire today
	query="select count(id) from cards where combinedDue < " + FormatHelper::GetTimeStr() + " and priority in (1,2,3,4) and type = 1"; 
	SQLiteHelper::values.clear();
	retCode = sqlite3_exec(dbDeck, query.c_str(), callback, 0, &zErrMsg);
	if( retCode!=SQLITE_OK )
	{
		std::cout << "SQL error: " <<  zErrMsg << std::endl;
		sqlite3_free(zErrMsg);
	}
	numCardsReviewToday=FormatHelper::StrToInt(SQLiteHelper::values[0]["count(id)"]);
	std::cout<<"REVIEW cards today = "<< numCardsReviewToday <<std::endl;


	//close sqlite
	sqlite3_close(dbDeck);
}

void DeckAnki::LoadData()
{
	std::cout<<"Loading deck data " <<std::endl;
	//update databse entriies
	Fetch();
}

void DeckAnki::AnswerCard(Answer answer)
{
	std::cout<<"Registering answer" <<std::endl;
	switch (answer)
	{
		case 0:
			cardsDueBuffer.push_back(*lastCard);
			
			break;
		default:
			numCardsFailedToday--;
			break;
	}
	
}
