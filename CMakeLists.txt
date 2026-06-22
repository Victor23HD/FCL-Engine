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

find_package(Threads REQUIRED)

add_library(fcl_engine STATIC
    src/flight/flight_control_engine.cpp
    src/sensors/sensor_worker.cpp
    src/sensors/virtual_hydraulic_sensor.cpp
    src/voting/tmr_voter.cpp
    src/watchdog/watchdog_monitor.cpp
)

target_link_libraries(fcl_engine PUBLIC fcl_core Threads::Threads)

add_executable(fcl_hello src/main.cpp)
target_link_libraries(fcl_hello PRIVATE fcl_engine)

option(FCL_BUILD_TESTS "Build unit tests" ON)

if(FCL_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(fcl_tests
        tests/test_tmr_voter.cpp
        tests/test_spsc_queue.cpp
        tests/test_heartbeat.cpp
        tests/test_flight_engine.cpp
    )

    target_link_libraries(fcl_tests PRIVATE fcl_engine GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(fcl_tests)
endif()
