cmake_minimum_required(VERSION 3.20)

project(FCL-Engine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(fcl_core INTERFACE)
target_include_directories(fcl_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sensors
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_executable(fcl_hello
    src/main.cpp
    src/sensors/virtual_hydraulic_sensor.cpp
    src/voting/tmr_voter.cpp
)

target_link_libraries(fcl_hello PRIVATE fcl_core)
