#include "libs/sqlite3.h"
#include "utils.hpp"
#include "exceptions.hpp"
#include <map>
#include <vector>


typedef std::map<std::string,std::string> StringMap;
// ------------------------- SQLite routines ------------------------
static int callback(void *NotUsed, int argc, char **argv, char **azColName);

class SQLiteHelper //should be changed with database class
{
public:
	static std::vector<StringMap> values;
	static void SQLiteHelper::ExecuteQuery(sqlite3 * dbDeck, std::string query);
};
std::vector<StringMap> SQLiteHelper::values;


void SQLiteHelper::ExecuteQuery(sqlite3 * dbDeck, std::string query)
{
	char *zErrMsg = NULL;
	int retCode;
	values.clear();
	retCode = sqlite3_exec(dbDeck, query.c_str(), callback, 0, &zErrMsg);
	if( retCode!=SQLITE_OK )
	{
		Exception exceprion( FormatHelper::ConvertToStr("SQL error: ") + FormatHelper::ConvertToStr(zErrMsg) );
		sqlite3_free(zErrMsg);
	}


}

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

