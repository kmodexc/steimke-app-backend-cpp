add_subdirectory(modules/googletest2)
list(APPEND GTEST_SOURCES itemtests.cpp)
add_executable(gtest_all main.cpp ${GTEST_SOURCES})
target_link_libraries(gtest_all PUBLIC rlservlib gtest_main)