cmake_minimum_required(VERSION 3.10.0)
project(rlserv VERSION 0.2.0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(CmakeConfig.h.in CmakeConfig.h)

set(JSON_BuildTests OFF CACHE INTERNAL "")

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_MODULE_PATH 
	${CMAKE_MODULE_PATH} 
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake" 
	"${CMAKE_CURRENT_SOURCE_DIR}/rlservlib/modules/libhttpserver/cmakemodule")
include(CPack)

add_subdirectory(rlservlib)
add_subdirectory(gtest)

add_executable(rlserv main.cpp )

target_include_directories(rlserv PUBLIC include ${PROJECT_BINARY_DIR})
target_link_libraries(rlserv PUBLIC rlservlib spdlog)

install(TARGETS rlserv DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/CmakeConfig.h" DESTINATION include)

add_test(NAME TEST_ALL COMMAND gtest/gtest_all)

