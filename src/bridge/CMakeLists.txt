file(GLOB_RECURSE MQTTBRIDGE_SOURCES CONFIGURE_DEPENDS *.cc)

add_library(bridge OBJECT ${MQTTBRIDGE_SOURCES})
target_include_directories(bridge PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${mosquitto_INCLUDE_DIRS}
    ${tomlplusplus_INCLUDE_DIRS}
    ${fmt_INCLUDE_DIRS})

target_link_libraries(bridge PUBLIC 
    Boost::log 
    Boost::log_setup)

set_target_properties(bridge PROPERTIES POSITION_INDEPENDENT_CODE ON)