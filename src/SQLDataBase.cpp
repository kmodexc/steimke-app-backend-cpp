#include "SQLDataBase.h"

using namespace std;

namespace rls
{

SQLDataBase::SQLDataBase()
{
	std::string fqn = db_file_path + db_file_name;
	int rc = sqlite3_open(fqn.c_str(), &db);
	if (rc)
	{
		cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		db = nullptr;
		return;
	}
}
bool SQLDataBase::execute(std::string command)
{
	if (db == nullptr)
		return false;
	char *zErrMsg = 0;
	int rc = sqlite3_exec(db, command.c_str(), callback, this, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cerr << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);
		db = nullptr;
		return false;
	}
	return true;
}
static int callback(void *userdata, int argc, char **argv, char **azColName)
{
	SQLDataBase* mydb = (SQLDataBase*)userdata;
	
}
Item SQLDataBase::getItem(int id)
{
}
std::vector<Item> SQLDataBase::getItems()
{
}
SQLDataBase::~SQLDataBase()
{
	if (db != nullptr)
		sqlite3_close(db);
}

} // namespace rls