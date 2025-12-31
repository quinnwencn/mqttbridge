cmake_minimum_required(VERSION 3.20)

project(xbridge VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED system log log_setup filesystem)
find_package(tomlplusplus REQUIRED)
find_package(fmt REQUIRED)
find_package(CLI11 REQUIRED)
find_package(mosquitto REQUIRED)

add_subdirectory(src)
add_subdirectory(examples)

add_executable(${PROJECT_NAME} src/main.cc)
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC
    mqttbridge
    CLI11::CLI11)