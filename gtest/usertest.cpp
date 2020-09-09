#include "gtest/gtest.h"
#include "App.h"
#include "MockConHandle.h"
#include "DependencyService.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

TEST(UserTest,UserTest1){
	try{
			spdlog::basic_logger_mt("rlservlib","logs/basic-file.log");
	}catch(spdlog::spdlog_ex){

	}
	SPDLOG_DEBUG("");
	rls::App app;
	char portstr[] = "9999";
	char *arr[] = {nullptr, portstr};
	EXPECT_TRUE(app.initialize(2, arr));
	rls::MockConHandle con;

	// create string rep of item
	std::string contentstr = "";
	rls::DependencyService dep;
	auto jsonconv = dep.getJSONSerializer();
	contentstr = jsonconv->toJSON(rls::User(23,"testuser","pw","em",rls::UserState::user,0));

	EXPECT_TRUE(app.post(&con,"/api/user",contentstr));
	EXPECT_EQ(con.send_content,"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: 12\r\n\r\npost user ok");
	con.send_content.clear();
	EXPECT_TRUE(app.del(&con,"/api/user/23"));
	EXPECT_EQ(con.send_content,"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: 11\r\n\r\ndel user ok");
	con.send_content.clear();
}

TEST(UserTest,UserTestMod){
	try{
			spdlog::basic_logger_mt("rlservlib","logs/basic-file.log");
	}catch(spdlog::spdlog_ex){

	}
	SPDLOG_DEBUG("");
	rls::App app;
	char portstr[] = "9999";
	char *arr[] = {nullptr, portstr};
	EXPECT_TRUE(app.initialize(2, arr));
	rls::MockConHandle con;

	// create string rep of item
	std::string contentstr = "";
	rls::DependencyService dep;
	auto jsonconv = dep.getJSONSerializer();
	rls::User user(23,"testuser","pw","em",rls::UserState::mod,0);
	contentstr = jsonconv->toJSON(user);

	EXPECT_TRUE(app.post(&con,"/api/user",contentstr));
	EXPECT_EQ(con.send_content,"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: 12\r\n\r\npost user ok");
	con.send_content.clear();

	EXPECT_TRUE(app.get(&con,"/api/user/23"));
	EXPECT_EQ(con.send_content,std::string("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: 77\r\n\r\n")+contentstr);
	con.send_content.clear();

	rls::User user2;
	jsonconv->fromJSON(contentstr,&user2);

	EXPECT_EQ(user2.getId(),user.getId());
	EXPECT_EQ(user2.getName(),user.getName());
	EXPECT_EQ(user2.getState(),user.getState());

	// eventually make operator== for user
	//EXPECT_EQ(user2,user);

	EXPECT_TRUE(app.del(&con,"/api/user/23"));
	EXPECT_EQ(con.send_content,"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: 11\r\n\r\ndel user ok");
	con.send_content.clear();
}

TEST(UserTest,UserTestIds){
	try{
			spdlog::basic_logger_mt("rlservlib","logs/basic-file.log");
	}catch(spdlog::spdlog_ex){

	}
	SPDLOG_DEBUG("");
	rls::App app;
	char portstr[] = "20999";
	char *arr[] = {nullptr, portstr};
	EXPECT_TRUE(app.initialize(2, arr));
	rls::MockConHandle con;

	// create string rep of item
	std::string contentstr = "";
	rls::DependencyService dep;
	auto jsonconv = dep.getJSONSerializer();
	contentstr = jsonconv->toJSON(rls::User(23,"testuser","pw","em",rls::UserState::user,0));

	EXPECT_TRUE(app.post(&con,"/api/user",contentstr));
	EXPECT_EQ(con.send_content,"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: 12\r\n\r\npost user ok");
	con.send_content.clear();

	EXPECT_TRUE(app.get(&con,"/api/users"));
	EXPECT_EQ(con.send_content,"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: 4\r\n\r\n[23]");
	con.send_content.clear();

	EXPECT_TRUE(app.del(&con,"/api/user/23"));
	EXPECT_EQ(con.send_content,"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: 11\r\n\r\ndel user ok");
	con.send_content.clear();
}