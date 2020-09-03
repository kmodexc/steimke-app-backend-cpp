cmake_minimum_required(VERSION 3.10.0)
project(rlservlib VERSION 0.1.0)
cmake_policy(SET CMP0079 NEW)


list(APPEND RLSERV_SOURCES App.cpp)
list(APPEND RLSERV_SOURCES DependencyService.cpp)

add_library(rlservlib ${RLSERV_SOURCES})

add_subdirectory(modules)
add_subdirectory(models)
add_subdirectory(datastore)
add_subdirectory(jsonconv)
add_subdirectory(webservice)

list(APPEND EXTRA_LIBS ${CMAKE_DL_LIBS})
list(APPEND EXTRA_LIBS DataStore)
list(APPEND EXTRA_LIBS JsonConv)
list(APPEND EXTRA_LIBS Models)
list(APPEND EXTRA_LIBS WebService)

target_link_libraries(rlservlib PUBLIC ${EXTRA_LIBS})

target_include_directories(rlservlib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS rlservlib DESTINATION lib)