add_executable(transport_test main.cc)
target_link_libraries(transport_test PUBLIC 
    mqttbridge 
    ${tomlplusplus_LIBRARIES}
     ${Boost_LIBRARIES})
target_include_directories(transport_test PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${tomlplusplus_INCLUDE_DIRS})