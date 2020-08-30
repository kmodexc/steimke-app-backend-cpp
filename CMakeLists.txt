cmake_minimum_required(VERSION 3.0.0)
project(rlserv VERSION 0.1.0)

set(JSON_BuildTests OFF CACHE INTERNAL "")

include(CTest)
enable_testing()

add_executable(rlserv main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CPack)

add_subdirectory(modules/json)

target_link_libraries(rlserv PRIVATE nlohmann_json::nlohmann_json)

find_package (SQLite3 REQUIRED)
if (SQLITE3_FOUND)
  include_directories(${SQLITE3_INCLUDE_DIRS})
  target_link_libraries (tutorial ${SQLITE3_LIBRARIES})
endif (SQLITE3_FOUND)
