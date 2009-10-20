//deckAnki
#include "deckAnki.hpp"
#include "libs/sqlite3.h"
#include <iostream>
#include <map>
#include <vector>



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
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	SQLiteResults::values.push_back(newMap);
	printf("\n");
	return 0;
}

//--------------------------- DeckAnki ------------------------------
Card DeckAnki::getNextCard()
{

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
		Card cardError(CardField("Can't open database: "),CardField(sqlite3_errmsg(dbDeck)));
		return cardError;
		//throw exception here?
	}
	std::cout<< "reading sqlite  "<< fileName <<" " <<retCode<<std::endl;
	std::string query="SELECT * FROM cards ORDER BY RANDOM() LIMIT 1";
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
	return card;
}