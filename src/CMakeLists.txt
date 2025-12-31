add_subdirectory(config)
add_subdirectory(domainsocket)
add_subdirectory(logger)
add_subdirectory(bridge)
add_subdirectory(mqttclient)
add_subdirectory(transport)

add_library(mqttbridge SHARED
    $<TARGET_OBJECTS:config>
    $<TARGET_OBJECTS:domainsocket>
    $<TARGET_OBJECTS:logger>
    $<TARGET_OBJECTS:bridge>
    $<TARGET_OBJECTS:mqttclient>
    $<TARGET_OBJECTS:transport>
)

target_link_libraries(mqttbridge PUBLIC 
    fmt::fmt
    mosquitto::mosquitto
    Boost::log 
    Boost::log_setup 
    tomlplusplus::tomlplusplus)