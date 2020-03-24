#include "SQLDataBase.h"

#define CHECK_SQL_ERROR(returncode, errorRetVal)             \
	if (returncode != SQLITE_OK)                             \
	{                                                        \
		cerr << "SQL error: " << sqlite3_errmsg(db) << endl; \
		sqlite3_close(db);                                   \
		db = nullptr;                                        \
		return errorRetVal;                                  \
	}

using namespace std;

namespace rls
{

typedef std::unique_lock<std::mutex> stdlock;

SQLDataBase::SQLDataBase()
{
	stdlock(mtx);
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
void SQLDataBase::addItem(Item it)
{
	if (db == nullptr)
	{
		cerr << "Database closed" << endl;
		return;
	}
	stdlock(mtx);

	const char command[] = "INSERT INTO Items (ID,state,name,shortdesc,desc,creaid,assid,prio,wl) VALUES (?1,?2,?3,?4,?5,?6,?7,?8,?9)";
	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare_v2(db, command, sizeof(command), &stmt, nullptr);
	CHECK_SQL_ERROR(rc,);

	rc = sqlite3_bind_int(stmt, 1, it.getID());
	CHECK_SQL_ERROR(rc,);
	rc = sqlite3_bind_int(stmt, 2, (int)it.getState());
	CHECK_SQL_ERROR(rc,);
	rc = sqlite3_bind_text(stmt, 3, it.getName().c_str(),it.getName().length(),SQLITE_TRANSIENT);
	CHECK_SQL_ERROR(rc,);
	rc = sqlite3_bind_text(stmt, 4, it.getShortDiscription().c_str(),it.getShortDiscription().length(),SQLITE_TRANSIENT);
	CHECK_SQL_ERROR(rc,);
	rc = sqlite3_bind_text(stmt, 5, it.getDescription().c_str(),it.getDescription().length(),SQLITE_TRANSIENT);
	CHECK_SQL_ERROR(rc,);
	rc = sqlite3_bind_int(stmt, 6, it.getCreatorID());
	CHECK_SQL_ERROR(rc,);
	rc = sqlite3_bind_int(stmt, 7, it.getAssignedID());
	CHECK_SQL_ERROR(rc,);
	rc = sqlite3_bind_int(stmt, 8, it.getPriority());
	CHECK_SQL_ERROR(rc,);
	rc = sqlite3_bind_int(stmt, 9, it.getWorkload());
	CHECK_SQL_ERROR(rc,);

	rc = sqlite3_step(stmt);
	CHECK_SQL_ERROR(rc,);

	rc = sqlite3_finalize(stmt);
	CHECK_SQL_ERROR(rc,);
}
Item SQLDataBase::getItem(int id)
{
	if (db == nullptr)
	{
		cerr << "Database closed" << endl;
		return Item();
	}
	stdlock(mtx);

	const char command[] = "SELECT * WHERE ID=?1";
	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare_v2(db, command, sizeof(command), &stmt, nullptr);
	CHECK_SQL_ERROR(rc, Item());

	rc = sqlite3_bind_int(stmt, 1, id);
	CHECK_SQL_ERROR(rc, Item());

	rc = sqlite3_step(stmt);
	CHECK_SQL_ERROR(rc, Item());

	ItemState state = (ItemState)sqlite3_column_int(stmt, 1);
	std::string name = (const char *)sqlite3_column_text(stmt, 2);
	std::string shortdesc = (const char *)sqlite3_column_text(stmt, 3);
	std::string desc = (const char *)sqlite3_column_text(stmt, 4);
	int creaid = sqlite3_column_int(stmt, 5);
	int assid = sqlite3_column_int(stmt, 6);
	int prio = sqlite3_column_int(stmt, 7);
	int wl = sqlite3_column_int(stmt, 8);

	Item it(id, state, name, shortdesc, desc, assid, creaid, prio, wl);

	rc = sqlite3_finalize(stmt);
	CHECK_SQL_ERROR(rc, Item());

	return it;
}
std::vector<int> SQLDataBase::getIDs()
{
	if (db == nullptr)
	{
		cerr << "Database closed" << endl;
		return std::vector<int>();
	}
	stdlock(mtx);

	char command[] = "SELECT ID FROM Items";
	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare_v2(db, command, sizeof(command), &stmt, nullptr);
	CHECK_SQL_ERROR(rc, std::vector<int>());

	std::vector<int> retval;

	do{
		CHECK_SQL_ERROR(rc, std::vector<int>());
		rc = sqlite3_step(stmt);
		retval.push_back(sqlite3_column_int(stmt,0));
	}while(rc != SQLITE_DONE);

	rc = sqlite3_finalize(stmt);
	CHECK_SQL_ERROR(rc, std::vector<int>());

	return retval;
}
std::vector<Item> SQLDataBase::getItems()
{
	// dont invoke mutex here
	auto ids = getIDs();

	std::vector<Item> retval;

	for(auto id : ids){
		retval.push_back(getItem(id));
	}

	return retval;
}
void SQLDataBase::updateItem(Item it)
{
	// dont invoke mutex here
	deleteItem(it.getID());
	addItem(it);
}
void SQLDataBase::deleteItem(int id)
{
	if (db == nullptr)
	{
		cerr << "Database closed" << endl;
		return;
	}
	stdlock(mtx);

	char command[] = "DELETE FROM Items WHERE ID=?1";
	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare_v2(db, command, sizeof(command), &stmt, nullptr);
	CHECK_SQL_ERROR(rc,);

	rc = sqlite3_bind_int(stmt, 1, id);
	CHECK_SQL_ERROR(rc,);

	rc = sqlite3_step(stmt);
	CHECK_SQL_ERROR(rc,);

	rc = sqlite3_finalize(stmt);
	CHECK_SQL_ERROR(rc,);
}
SQLDataBase::~SQLDataBase()
{
	mtx.try_lock();	// dont really lock here to try safe db in case of exception
	if (db != nullptr)
		sqlite3_close(db);
}

} // namespace rls