#include "SQLDataBaseItem.h"
#include "spdlog/spdlog.h"

#define CHECK_SQL_ERROR(returncode, errorRetVal)                                          \
	if (returncode != SQLITE_OK && returncode != SQLITE_DONE && returncode != SQLITE_ROW) \
	{                                                                                     \
		spdlog::error("SQL error({}): {}",returncode,sqlite3_errmsg(db));			      \
		sqlite3_close(db);                                                                \
		db = nullptr;                                                                     \
		return errorRetVal;                                                               \
	}

#define TO_CPP_STRING(in) (in == nullptr ? std::string("null") : (const char *)in)

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
							   "wl			int		,"
							   "placeid		int		,"
							   "res1		int		,"
							   "res2		int		,"
							   "res3		int		,"
							   "res4		int		,"
							   "creayear	int		,"
							   "creamonth	int		,"
							   "creaday		int		,"
							   "creahour	int		,"
							   "creaminute	int		,"
							   "res5		int		,"
							   "assyear		int		,"
							   "assmonth	int		,"
							   "assday		int		,"
							   "asshour		int		,"
							   "assminute	int		,"
							   "res6		int		,"
							   "finyear		int		,"
							   "finmonth	int		,"
							   "finday		int		,"
							   "finhour		int		,"
							   "finminute	int		,"
							   "res7		int		)";

		char *errmsg;
		if (sqlite3_exec(db, command, 0, nullptr, &errmsg) != SQLITE_OK)
		{
			
			if (errmsg)
			{
				spdlog::error("error creating table; message={}",errmsg);
			}else{
				spdlog::error("error creating table; nomsg");
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
		spdlog::error("Database closed");
		return;
	}
	stdlock lock(mtx);

	const char command[] = "INSERT INTO Items "
						   "(ID,state,name,shortdesc,desc,creaid,assid,prio,wl,placeid,res1,res2,res3,"
						   "res4,creayear,creamonth,creaday,creahour,creaminute,"
						   "res5,assyear,assmonth,assday,asshour,assminute,"
						   "res6,finyear,finmonth,finday,finhour,finminute,"
						   "res7) "
						   "VALUES (?1,?2,?3,?4,?5,?6,?7,?8,?9,?10,?11,?12,?13,?14,?15,"
						   "?16,?17,?18,?19,?20,?21,?22,?23,?24,?25,?26,?27,?28,?29,?30,?31,?32)";
	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare_v2(db, command, sizeof(command), &stmt, nullptr);
	CHECK_SQL_ERROR(rc, );

	// parameters start at 1

	// base data

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
	rc = sqlite3_bind_int(stmt, 10, it.getPlaceID());
	CHECK_SQL_ERROR(rc, );

	// some reserved

	rc = sqlite3_bind_int(stmt, 11, 0);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, 12, 0);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, 13, 0);
	CHECK_SQL_ERROR(rc, );

	// created

	rc = sqlite3_bind_int(stmt, 14, 0);
	CHECK_SQL_ERROR(rc, );

	rc = sqlite3_bind_int(stmt, 15, it.getCreateTime().year);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, 16, it.getCreateTime().month);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, 17, it.getCreateTime().day);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, 18, it.getCreateTime().hour);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, 19, it.getCreateTime().minute);
	CHECK_SQL_ERROR(rc, );

	// assigned

	rc = sqlite3_bind_int(stmt, 20, 0);
	CHECK_SQL_ERROR(rc, );

	rc = sqlite3_bind_int(stmt, 21, it.getAssignedTime().year);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, 22, it.getAssignedTime().month);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, 23, it.getAssignedTime().day);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, 24, it.getAssignedTime().hour);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, 25, it.getAssignedTime().minute);
	CHECK_SQL_ERROR(rc, );

	// finished

	rc = sqlite3_bind_int(stmt, 26, 0);
	CHECK_SQL_ERROR(rc, );

	rc = sqlite3_bind_int(stmt, 27, it.getFinishedTime().year);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, 28, it.getFinishedTime().month);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, 29, it.getFinishedTime().day);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, 30, it.getFinishedTime().hour);
	CHECK_SQL_ERROR(rc, );
	rc = sqlite3_bind_int(stmt, 31, it.getFinishedTime().minute);
	CHECK_SQL_ERROR(rc, );

	rc = sqlite3_bind_int(stmt, 32, 0);
	CHECK_SQL_ERROR(rc, );

	

	// execute

	rc = sqlite3_step(stmt);
	CHECK_SQL_ERROR(rc, );

	rc = sqlite3_finalize(stmt);
	CHECK_SQL_ERROR(rc, );
}
Item SQLDataBaseItem::get(int id)
{
	if (db == nullptr)
	{
		spdlog::error("Database closed");
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

	// colum starts at 0

	// int id = sqlite3_column_int(stmt, 0);
	ItemState state = (ItemState)sqlite3_column_int(stmt, 1);
	std::string name = TO_CPP_STRING(sqlite3_column_text(stmt, 2));
	std::string shortdesc = TO_CPP_STRING(sqlite3_column_text(stmt, 3));
	std::string desc = TO_CPP_STRING(sqlite3_column_text(stmt, 4));
	int creaid = sqlite3_column_int(stmt, 5);
	int assid = sqlite3_column_int(stmt, 6);
	int prio = sqlite3_column_int(stmt, 7);
	int wl = sqlite3_column_int(stmt, 8);
	int placeid = sqlite3_column_int(stmt, 9);

	//int res1 = sqlite3_column_int(stmt, 10);
	//int res2 = sqlite3_column_int(stmt, 11);
	//int res3 = sqlite3_column_int(stmt, 12);	

	//int res4 = sqlite3_column_int(stmt, 13);

	int crea_year = sqlite3_column_int(stmt, 14);
	int crea_month = sqlite3_column_int(stmt, 15);
	int crea_day = sqlite3_column_int(stmt, 16);
	int crea_hour = sqlite3_column_int(stmt, 17);
	int crea_minute = sqlite3_column_int(stmt, 18);

	//int res5 = sqlite3_column_int(stmt, 19);

	int ass_year = sqlite3_column_int(stmt, 20);
	int ass_month = sqlite3_column_int(stmt, 21);
	int ass_day = sqlite3_column_int(stmt, 22);
	int ass_hour = sqlite3_column_int(stmt, 23);
	int ass_minute = sqlite3_column_int(stmt, 24);

	//int res6 = sqlite3_column_int(stmt, 25);

	int fin_year = sqlite3_column_int(stmt, 26);
	int fin_month = sqlite3_column_int(stmt, 27);
	int fin_day = sqlite3_column_int(stmt, 28);
	int fin_hour = sqlite3_column_int(stmt, 29);
	int fin_minute = sqlite3_column_int(stmt, 30);

	//int res7 = sqlite3_column_int(stmt, 31);

	TimeStamp createTime;
	createTime.year = crea_year;
	createTime.month = crea_month;
	createTime.day = crea_day;
	createTime.hour = crea_hour;
	createTime.minute = crea_minute;

	TimeStamp assignedTime;
	assignedTime.year = ass_year;
	assignedTime.month = ass_month;
	assignedTime.day = ass_day;
	assignedTime.hour = ass_hour;
	assignedTime.minute = ass_minute;

	TimeStamp finishedTime;
	finishedTime.year = fin_year;
	finishedTime.month = fin_month;
	finishedTime.day = fin_day;
	finishedTime.hour = fin_hour;
	finishedTime.minute = fin_minute;

	Item it(id, state, name, shortdesc, desc, assid, creaid, prio, wl,placeid,
		createTime,assignedTime,finishedTime);

	rc = sqlite3_finalize(stmt);
	CHECK_SQL_ERROR(rc, Item());

	return it;
}
std::vector<int> SQLDataBaseItem::getIDs()
{
	if (db == nullptr)
	{
		spdlog::error("Database closed");
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
		if (rc == SQLITE_ROW)
			retval.push_back(sqlite3_column_int(stmt, 0));
	} while (rc != SQLITE_DONE);

	rc = sqlite3_finalize(stmt);
	CHECK_SQL_ERROR(rc, std::vector<int>());
	spdlog::debug("Found {} Items in DB", retval.size());
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
	spdlog::debug("Return {} Items",retval.size());
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
		spdlog::debug("Database closed");
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