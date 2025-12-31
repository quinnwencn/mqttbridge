add_executable(server main.cc)
target_link_libraries(server domainsocket)
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/src)
