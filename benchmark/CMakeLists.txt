include(FetchContent)

FetchContent_Declare(
    nanobench
    GIT_REPOSITORY https://github.com/martinus/nanobench.git
    GIT_TAG v4.1.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(nanobench)

add_executable(${PROJECT_NAME}-benchmark
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

target_include_directories(${PROJECT_NAME}-benchmark PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(${PROJECT_NAME}-benchmark PROPERTIES
    CXX_STANDARD          20
    CXX_STANDARD_REQUIRED TRUE
)

target_link_libraries(${PROJECT_NAME}-benchmark
    ${PROJECT_NAME}
    nanobench
)