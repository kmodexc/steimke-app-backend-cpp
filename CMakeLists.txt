cmake_minimum_required(VERSION 3.10.0)
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

add_subdirectory(modules)


list(APPEND EXTRA_INCLUDES ${PROJECT_BINARY_DIR})
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/modules/libhttp/include")
list(APPEND EXTRA_LIBS "${PROJECT_SOURCE_DIR}/modules/libhttp/lib/libhttp.a")
list(APPEND EXTRA_LIBS nlohmann_json::nlohmann_json)
list(APPEND EXTRA_LIBS pthread)
list(APPEND EXTRA_LIBS ${CMAKE_DL_LIBS})
list(APPEND EXTRA_LIBS SQLite3)

list(APPEND SOURCES src/App.cpp)
list(APPEND SOURCES src/ConHandle.cpp)
list(APPEND SOURCES src/DBItem.cpp)
list(APPEND SOURCES src/DependencyService.cpp)
list(APPEND SOURCES src/HTTPSocket.cpp)
list(APPEND SOURCES src/Item.cpp)
list(APPEND SOURCES src/JSONSerializer.cpp)
list(APPEND SOURCES src/MockDataStore.cpp)
list(APPEND SOURCES src/Place.cpp)
list(APPEND SOURCES src/SimpleConHandle.cpp)
list(APPEND SOURCES src/SimpleSocket.cpp)
list(APPEND SOURCES src/SQLDataBaseItem.cpp)
list(APPEND SOURCES src/SQLDataBasePlaces.cpp)
list(APPEND SOURCES src/SQLDataBaseUser.cpp)
list(APPEND SOURCES src/User.cpp)

add_executable(rlserv src/main.cpp ${SOURCES})

target_include_directories(rlserv PUBLIC include ${EXTRA_INCLUDES})
target_link_libraries(rlserv PUBLIC  ${EXTRA_LIBS})


install(TARGETS rlserv DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/CmakeConfig.h" DESTINATION include)

