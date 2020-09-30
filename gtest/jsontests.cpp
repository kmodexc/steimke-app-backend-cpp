#include "gtest/gtest.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "JSONSerializer.h"

using namespace rls;
using json = nlohmann::json;

TEST(TestJSONSer, Test1)
{
	try{
			spdlog::basic_logger_mt("rlservlib","logs/basic-file.log");
	}catch(spdlog::spdlog_ex){

	}
	JSONSerializer ser;
	json j1;
	j1["ident1"] = "val1";
	j1["ident2"] = "val2";
	json j2;
	j2["ident3"] = j1;
	j2["ident4"] = "val4";
	EXPECT_EQ(j2.dump(),"{\"ident3\":{\"ident1\":\"val1\",\"ident2\":\"val2\"},\"ident4\":\"val4\"}");
}

TEST(TestJSONSer, TestGetAll1)
{
	try{
			spdlog::basic_logger_mt("rlservlib","logs/basic-file.log");
	}catch(spdlog::spdlog_ex){

	}

	ContainerItemPlaceUser cont;
	Item it(1, ItemState::created, "testitem1" ,
			 "shortdes", "desc",
			 0, 0, 0, 0, 0,
			 TimeStamp(), TimeStamp(), TimeStamp());
	cont.items.push_back(it);
	Item it2(2, ItemState::created, "testitem2" ,
			 "shortdes", "desc",
			 0, 0, 0, 0, 0,
			 TimeStamp(), TimeStamp(), TimeStamp());
	cont.items.push_back(it2);

	Place p1;
	p1.id = 1;
	p1.name = "testplace1";
	p1.creatorId = 1;
	cont.places.push_back(p1);
	Place p2;
	p2.id = 2;
	p2.name = "testplace2";
	p2.creatorId = 2;
	cont.places.push_back(p2);

	User u1(1,"testuser1","","",UserState::guest,0);
	cont.users.push_back(u1);
	User u2(2,"testuser2","","",UserState::guest,0);
	cont.users.push_back(u2);

	JSONSerializer ser;
	std::string content = ser.toJSON(cont);
	
	EXPECT_EQ(content,"{\"items\":[{\"assignedid\":0,\"assignedtime\":{\"day\":0,\"hour\":0,\"minute\":0,\"month\":0,\"year\":0},\"createtime\":{\"day\":0,\"hour\":0,\"minute\":0,\"month\":0,\"year\":0},\"creatorid\":0,\"desc\":\"desc\",\"finishedtime\":{\"day\":0,\"hour\":0,\"minute\":0,\"month\":0,\"year\":0},\"id\":1,\"name\":\"testitem1\",\"placeid\":0,\"priority\":0,\"shortdesc\":\"shortdes\",\"state\":\"created\",\"workload\":0},{\"assignedid\":0,\"assignedtime\":{\"day\":0,\"hour\":0,\"minute\":0,\"month\":0,\"year\":0},\"createtime\":{\"day\":0,\"hour\":0,\"minute\":0,\"month\":0,\"year\":0},\"creatorid\":0,\"desc\":\"desc\",\"finishedtime\":{\"day\":0,\"hour\":0,\"minute\":0,\"month\":0,\"year\":0},\"id\":2,\"name\":\"testitem2\",\"placeid\":0,\"priority\":0,\"shortdesc\":\"shortdes\",\"state\":\"created\",\"workload\":0}],\"places\":[{\"creatorId\":1,\"id\":1,\"members\":[],\"name\":\"testplace1\",\"type\":\"public\"},{\"creatorId\":2,\"id\":2,\"members\":[],\"name\":\"testplace2\",\"type\":\"public\"}],\"users\":[{\"email\":\"\",\"id\":1,\"name\":\"testuser1\",\"pw\":\"\",\"state\":\"guest\",\"workload\":0},{\"email\":\"\",\"id\":2,\"name\":\"testuser2\",\"pw\":\"\",\"state\":\"guest\",\"workload\":0}]}");
}

TEST(TestJSONSer, TestGetAllEmpty)
{
	try{
			spdlog::basic_logger_mt("rlservlib","logs/basic-file.log");
	}catch(spdlog::spdlog_ex){

	}

	ContainerItemPlaceUser cont;

	JSONSerializer ser;
	std::string content = ser.toJSON(cont);
	
	EXPECT_EQ(content,"{\"items\":[],\"places\":[],\"users\":[]}");
}