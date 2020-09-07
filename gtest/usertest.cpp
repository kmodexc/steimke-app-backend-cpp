#include "gtest/gtest.h"
#include "App.h"
#include "MockConHandle.h"
#include "DependencyService.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

TEST(UserTest,UserTest1){
	spdlog::basic_logger_mt("rlservlib","logs/basic-file.log");
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
	EXPECT_EQ(con.send_content,"ok user add");
	con.send_content.clear();
	EXPECT_TRUE(app.del(&con,"/api/user"));
	EXPECT_EQ(con.send_content,"ok user delete");
	con.send_content.clear();
}