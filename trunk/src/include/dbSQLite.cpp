#include "deckAnki.hpp"

std::vector<StringMap> SQLiteHelper::values;
static int callback(void *NotUsed, int argc, char **argv, char **azColName);

void SQLiteHelper::ExecuteQuery(sqlite3 * dbDeck, std::string query)
{
	char *zErrMsg = NULL;
	int retCode;
	values.clear();
	retCode = sqlite3_exec(dbDeck, query.c_str(), callback, 0, &zErrMsg);
	if( retCode!=SQLITE_OK )
	{
		Exception exception( FormatHelper::ConvertToStr("SQL error: ") + FormatHelper::ConvertToStr(zErrMsg) );
		sqlite3_free(zErrMsg);
		throw  exception;
	}


}

static int callback(void *NotUsed, int argc, char **argv, char **azColName)    //who does free for these poiners???????
{
	int i;
	//printf("NotUsed = %d\n", NotUsed);
	StringMap newMap;
	for(i=0; i<argc; i++){
		newMap.insert(std::pair<std::string,std::string>( azColName[i], argv[i] ? argv[i] : "NULL") );
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	SQLiteHelper::values.push_back(newMap);
	printf("\n");
	return 0;
}
