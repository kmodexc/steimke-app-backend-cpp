#include "DependencyService.h"
#include "App.h"
#include "MockConHandle.h"
#include "gtest/gtest.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

using namespace rls;

TEST(TestItemAll, ItemTest1)
{
	try{
			spdlog::basic_logger_mt("rlservlib","logs/basic-file.log");
	}catch(spdlog::spdlog_ex){

	}

	TimeStamp created, assigned, finished;
	created.year = 11;
	created.month = 12;
	created.day = 13;
	created.hour = 14;
	created.day = 15;
	assigned.year = 21;
	assigned.month = 22;
	assigned.day = 23;
	assigned.hour = 24;
	assigned.day = 25;
	finished.year = 31;
	finished.month = 32;
	finished.day = 33;
	finished.hour = 34;
	finished.day = 35;
	int creatorID = 1;
	Item it(234, ItemState::inprogress, "test", "shortdesc", "desc", 2, creatorID, 3, 4, 5, created, assigned, finished);

	// create string rep of item
	std::string contentstr = "";
	DependencyService dep;
	auto jsonconv = dep.getJSONSerializer();
	contentstr = jsonconv->toJSON(it);

	// add user to add this to make it valid
	auto userdb = dep.getDataBaseUser();
	User admin(1,"admin","pw","em",UserState::admin,0);
	userdb->add(admin);


	rls::App app;
	char portstr[] = "9999";
	char *arr[] = {nullptr, portstr};
	app.initialize(2, arr);

	MockConHandle con;

	EXPECT_TRUE(app.post(&con,"/api/item",contentstr));
	EXPECT_EQ(con.send_content,"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: 12\r\n\r\npost item ok");
	con.send_content.clear();
	EXPECT_TRUE(app.get(&con,"/api/item/234"));
	EXPECT_EQ(con.send_content,std::string("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: 352\r\n\r\n")+contentstr);
	con.send_content.clear();
	// cleanup
	EXPECT_TRUE(app.del(&con,"/api/item/234"));
	EXPECT_EQ(con.send_content,"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: 11\r\n\r\ndel item ok");
	con.send_content.clear();
	EXPECT_TRUE(app.del(&con,"/api/user/1"));
	EXPECT_EQ(con.send_content,"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: 11\r\n\r\ndel user ok");
}

TEST(TestItemDB, ItemAddGet)
{
	try{
			spdlog::basic_logger_mt("rlservlib","logs/basic-file.log");
	}catch(spdlog::spdlog_ex){

	}

	DependencyService depend;
	auto db = depend.getDataBaseItem();
	TimeStamp created, assigned, finished;
	created.year = 11;
	created.month = 12;
	created.day = 13;
	created.hour = 14;
	created.day = 15;
	assigned.year = 21;
	assigned.month = 22;
	assigned.day = 23;
	assigned.hour = 24;
	assigned.day = 25;
	finished.year = 31;
	finished.month = 32;
	finished.day = 33;
	finished.hour = 34;
	finished.day = 35;
	Item it(121, ItemState::created, "test", "shortdesc", "desc", 1, 2, 3, 4, 5, created, assigned, finished);
	db->add(it);
	auto it2 = db->get(it.getID());
	EXPECT_EQ(it.getID(), it2.getID());
	EXPECT_EQ(it.getName(), it2.getName());
	EXPECT_EQ(it.getShortDiscription(), it2.getShortDiscription());
	EXPECT_EQ(it.getDescription(), it2.getDescription());
	EXPECT_EQ(it.getAssignedID(), it2.getAssignedID());
	EXPECT_EQ(it.getCreatorID(), it2.getCreatorID());
	EXPECT_EQ(it.getPriority(), it2.getPriority());
	EXPECT_EQ(it.getWorkload(), it2.getWorkload());
	EXPECT_EQ(it.getPlaceID(), it2.getPlaceID());
	EXPECT_EQ(it.getAssignedTime(), it2.getAssignedTime());
	EXPECT_EQ(it.getCreateTime(), it2.getCreateTime());
	EXPECT_EQ(it.getFinishedTime(), it2.getFinishedTime());
	EXPECT_EQ(it.getState(), it2.getState());
}