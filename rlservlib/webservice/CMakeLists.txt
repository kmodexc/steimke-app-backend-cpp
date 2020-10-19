list(APPEND WEBSERVICE_SOURCES MockConHandle.cpp)
list(APPEND WEBSERVICE_SOURCES SimpleConHandle.cpp)
list(APPEND WEBSERVICE_SOURCES SimpleSocket.cpp)
list(APPEND WEBSERVICE_SOURCES SocketEtr.cpp)
#list(APPEND WEBSERVICE_SOURCES SocketYhirose.cpp)
add_library(WebService ${WEBSERVICE_SOURCES})
find_package(OpenSSL REQUIRED)
find_package(LibHttpServer REQUIRED)


target_include_directories(WebService INTERFACE 
	${LIBHTTPSERVER_INCLUDE_DIRS} 
	${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(WebService PUBLIC 
	Models 
	${LIBHTTPSERVER_LIBRARIES}
	OpenSSL::SSL 
	OpenSSL::Crypto 
	spdlog::spdlog 
	pthread)
