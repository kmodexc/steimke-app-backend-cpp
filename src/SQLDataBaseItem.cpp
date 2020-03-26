#include "SQLDataBaseItem.h"

#define CHECK_SQL_ERROR(returncode, errorRetVal)                                          \
	if (returncode != SQLITE_OK && returncode != SQLITE_DONE && returncode != SQLITE_ROW) \
	{                                                                                     \
		cerr << "SQL error(" << returncode << "): " << sqlite3_errmsg(db) << endl;        \
		sqlite3_close(db);                                                                \
		db = nullptr;                                                                     \
		return errorRetVal;                                                               \
	}

#define TO_CPP_STRING(in) (in == nullptr ? std::string("null") : (const char*)in)

using namespace std;

namespace rls
{

typedef std::unique_lock<std::mutex> stdlock;

SQLDataBaseItem::SQLDataBaseItem()
{
	stdlock lock(mtx);
	std::string fqn = db_file_path + db_file_name;

	// check if db already exists
	bool db_exist = false;
	ifstream dbfstr(fqn);
	if (dbfstr.good())
		db_exist = true;
	dbfstr.close();

	// open db
	int rc = sqlite3_open(fqn.c_str(), &db);
	CHECK_SQL_ERROR(rc, );

	if (!db_exist)
	{
		// create table if db was created
		const char command[] = "CREATE TABLE Items("
							   "ID 			INT		,"
							   "state		int		,"
							   "name		text	,"
							   "shortdesc	text	,"
							   "desc		text	,"
							   "creaid		int		,"
							   "assid		int		,"
							   "prio		int		,"
							   "wl			int		)";

		char *errmsg;
		if (sqlite3_exec(db, command, 0, nullptr, &errmsg) != SQLITE_OK)
		{
			cerr << "error creating table" << endl;
			if (errmsg)
			{
				cerr << "errormessage: " << errmsg << endl;
			}
			sqlite3_close(db);
			db = nullptr;
			return;
		}
	}
}
void SQLDataBaseItem::add(Item &it)
{
	if (db == nullptr)
	{
		cerr << "Database closed" << endl;
		return;
	}
	stdlock lock(mtx);

	const char command[] = "INSERT INTO Items (ID,state,name,shortdesc,desc,creaid,assid,prio,wl) "
						   "VALUES (?1,?2,?3,?4,?5,?6,?7,?8,?9)";
	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare_v2(db, command, sizeof(command), &stmt, nullptr);
	CHECK_SQL_ERROR(rc, );

	rc = sqlite3_bind_int(stmt, 1, it.getID());
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, 2, (int)it.getState());
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_text(stmt, 3, it.getName().c_str(), it.getName().length(), SQLITE_TRANSIENT);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_text(stmt, 4, it.getShortDiscription().c_str(), it.getShortDiscription().length(), SQLITE_TRANSIENT);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_text(stmt, 5, it.getDescription().c_str(), it.getDescription().length(), SQLITE_TRANSIENT);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, 6, it.getCreatorID());
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, 7, it.getAssignedID());
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, 8, it.getPriority());
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, 9, it.getWorkload());
	CHECK_SQL_ERROR(rc, );

	rc = sqlite3_step(stmt);
	CHECK_SQL_ERROR(rc, );

	rc = sqlite3_finalize(stmt);
	CHECK_SQL_ERROR(rc, );
}
Item SQLDataBaseItem::get(int id)
{
	if (db == nullptr)
	{
		cerr << "Database closed" << endl;
		return Item();
	}
	stdlock lock(mtx);

	const char command[] = "SELECT * FROM Items WHERE ID=?1";
	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare_v2(db, command, sizeof(command), &stmt, nullptr);
	CHECK_SQL_ERROR(rc, Item());

	rc = sqlite3_bind_int(stmt, 1, id);
	CHECK_SQL_ERROR(rc, Item());

	rc = sqlite3_step(stmt);
	CHECK_SQL_ERROR(rc, Item());

	ItemState state = (ItemState)sqlite3_column_int(stmt, 1);
	std::string name = TO_CPP_STRING(sqlite3_column_text(stmt, 2));
	std::string shortdesc = TO_CPP_STRING(sqlite3_column_text(stmt, 3));
	std::string desc = TO_CPP_STRING(sqlite3_column_text(stmt, 4));
	int creaid = sqlite3_column_int(stmt, 5);
	int assid = sqlite3_column_int(stmt, 6);
	int prio = sqlite3_column_int(stmt, 7);
	int wl = sqlite3_column_int(stmt, 8);

	Item it(id, state, name, shortdesc, desc, assid, creaid, prio, wl);

	rc = sqlite3_finalize(stmt);
	CHECK_SQL_ERROR(rc, Item());

	return it;
}
std::vector<int> SQLDataBaseItem::getIDs()
{
	if (db == nullptr)
	{
		cerr << "Database closed" << endl;
		return std::vector<int>();
	}
	stdlock lock(mtx);

	char command[] = "SELECT ID FROM Items";
	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare_v2(db, command, sizeof(command), &stmt, nullptr);
	CHECK_SQL_ERROR(rc, std::vector<int>());

	std::vector<int> retval;

	do
	{
		CHECK_SQL_ERROR(rc, std::vector<int>());
		rc = sqlite3_step(stmt);
		if(rc == SQLITE_ROW)
			retval.push_back(sqlite3_column_int(stmt, 0));
	} while (rc != SQLITE_DONE);

	rc = sqlite3_finalize(stmt);
	CHECK_SQL_ERROR(rc, std::vector<int>());
	cout << "Found " << retval.size() << " Item in DB" << endl;
	return retval;
}
std::vector<Item> SQLDataBaseItem::getAll()
{
	// dont invoke mutex here
	auto ids = getIDs();

	std::vector<Item> retval;

	for (auto id : ids)
	{
		retval.push_back(get(id));
	}
	cout << "Return " << retval.size() << " Item" << endl;
	return retval;
}
void SQLDataBaseItem::update(Item &it)
{
	// dont invoke mutex here
	del(it.getID());
	add(it);
}
void SQLDataBaseItem::del(int id)
{
	if (db == nullptr)
	{
		cerr << "Database closed" << endl;
		return;
	}
	stdlock lock(mtx);

	char command[] = "DELETE FROM Items WHERE ID=?1";
	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare_v2(db, command, sizeof(command), &stmt, nullptr);
	CHECK_SQL_ERROR(rc, );

	rc = sqlite3_bind_int(stmt, 1, id);
	CHECK_SQL_ERROR(rc, );

	rc = sqlite3_step(stmt);
	CHECK_SQL_ERROR(rc, );

	rc = sqlite3_finalize(stmt);
	CHECK_SQL_ERROR(rc, );
}
SQLDataBaseItem::~SQLDataBaseItem()
{
	mtx.try_lock(); // dont really lock here to try safe db in case of exception
	if (db != nullptr)
		sqlite3_close(db);
}

} // namespace rls