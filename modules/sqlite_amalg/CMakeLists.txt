add_library(SQLite3 sqlite3.c)
target_include_directories(SQLite3 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

