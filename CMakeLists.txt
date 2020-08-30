cmake_minimum_required(VERSION 3.0.0)
project(rlserv VERSION 0.1.0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(CmakeConfig.h.in CmakeConfig.h)

set(JSON_BuildTests OFF CACHE INTERNAL "")

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CPack)

add_subdirectory(modules/json)
add_subdirectory(src)