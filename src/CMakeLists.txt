add_library(domainsocket SHARED server.cc client.cc)
target_include_directories(domainsocket PUBLIC ${CMAKE_SOURCE_DIR}/include)