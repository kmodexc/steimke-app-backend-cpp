list(APPEND JSONCONV_SOURCES JSONSerializer.cpp)
add_library(JsonConv ${JSONCONV_SOURCES})
target_include_directories(JsonConv INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(JsonConv PUBLIC Models)
