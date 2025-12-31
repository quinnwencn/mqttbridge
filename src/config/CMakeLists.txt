file(GLOB_RECURSE CONFIG_SOURCES CONFIGURE_DEPENDS *.cc)

add_library(config OBJECT ${CONFIG_SOURCES})
target_include_directories(config PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${Boost_INCLUDE_DIRS}
        ${tomlplusplus_INCLUDE_DIRS}
)
target_link_libraries(config PUBLIC logger)

set_target_properties(config PROPERTIES POSITION_INDEPENDENT_CODE ON)
