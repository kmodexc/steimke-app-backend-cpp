add_subdirectory(sqlite_amalg)
add_subdirectory(json2)
add_subdirectory(spdlog)
#set(HTTPLIB_COMPILE ON)
#set(BUILD_SHARED_LIBS ON)
#set(HTTPLIB_REQUIRE_OPENSSL ON)
#add_subdirectory(cpp-httplib)

# find_path(LIBHTTPSERVER_INCLUDE_DIRS httpserver.hpp
# 	${CMAKE_CURRENT_SOURCE_DIR}/libhttpserver/src
#     /usr/include
#     /usr/include/httpserver
#     /usr/local/include/
#     /usr/local/include/httpserver
#     )

# find_library(LIBHTTPSERVER_LIBRARIES NAMES httpserver
# 	PATHS
# 	${CMAKE_CURRENT_SOURCE_DIR}/libhttpserver/build/src/.libs
#     /usr/lib
#     /usr/local/lib
#     /opt/local/lib
#     )