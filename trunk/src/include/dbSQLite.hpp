#ifndef MINDCRAFT_DBSQLITE
#define MINDCRAFT_DBSQLITE


#include "libs/sqlite3.h"
#include "utils.hpp"
#include "exceptions.hpp"
#include <map>
#include <vector>


typedef std::map<std::string,std::string> StringMap;
// ------------------------- SQLite routines ------------------------

class SQLiteHelper //should be changed with database class
{
public:
	static std::vector<StringMap> values;
	static void ExecuteQuery(sqlite3 * dbDeck, std::string query);
};





#endif // MINDCRAFT_DBSQLITE
