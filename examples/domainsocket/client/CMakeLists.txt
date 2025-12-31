add_executable(client main.cc)
target_link_libraries(client domainsocket)
target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/src)
