file(GLOB_RECURSE TRANSPORT_SRCS CONFIGURE_DEPENDS *.cc)

add_library(transport OBJECT ${TRANSPORT_SRCS})
target_include_directories(transport PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(transport PUBLIC domainsocket config logger)

set_target_properties(transport PROPERTIES POSITION_INDEPENDENT_CODE ON)