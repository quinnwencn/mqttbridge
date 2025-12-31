file(GLOB_RECURSE MQTT_SRCS CONFIGURE_DEPENDS *.cc)

add_library(mqttclient OBJECT ${MQTT_SRCS})
target_include_directories(mqttclient PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    ${mosquitto_INCLUDE_DIRS})
target_link_libraries(mqttclient PUBLIC transport config logger)
set_target_properties(mqttclient PROPERTIES POSITION_INDEPENDENT_CODE ON)