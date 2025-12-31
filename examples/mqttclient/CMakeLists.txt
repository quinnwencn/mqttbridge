add_executable(mqttclient_example main.cc)
target_link_libraries(mqttclient_example PUBLIC 
    mqttbridge)
target_include_directories(mqttclient_example PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${tomlplusplus_INCLUDE_DIRS})