cmake_minimum_required(VERSION 3.22)

project(constrained_type
    VERSION 0.0.0
    LANGUAGES CXX
)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_BENCHMARK "Build benchmark" ON)

add_library(${PROJECT_NAME} INTERFACE)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD          20
    CXX_STANDARD_REQUIRED TRUE
    COMPILE_FLAGS "-ffunction-sections"
)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if (BUILD_TESTS)
    message(STATUS "Building tests is enabled")
    add_subdirectory(tests)
else()
    message(STATUS "Building tests is disabled")
endif()

if (BUILD_BENCHMARK)
    message(STATUS "Building benchmark is enabled")
    add_subdirectory(benchmark)
else()
    message(STATUS "Building benchmark is disabled")
endif()
