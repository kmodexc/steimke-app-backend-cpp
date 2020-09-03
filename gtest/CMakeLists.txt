#cmake_minimum_required(VERSION 3.10.0)
#project(gtest_all VERSION 0.1.0)

#add_subdirectory(../rlservlib)

list(APPEND GTEST_SOURCES itemtests.cpp)

add_executable(gtest_all main.cpp ${GTEST_SOURCES})

target_link_libraries(gtest_all PUBLIC  rlservlib)