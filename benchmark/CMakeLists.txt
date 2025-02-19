include(FetchContent)

FetchContent_Declare(
    nanobench
    GIT_REPOSITORY https://github.com/martinus/nanobench.git
    GIT_TAG v4.1.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(nanobench)

function(add_benchmark target_name source_file)
    set(BENCHMARK_NAME "${PROJECT_NAME}-benchmark-${target_name}")
    set(BENCHMARK_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/${source_file}")
    add_executable(${BENCHMARK_NAME} ${BENCHMARK_SOURCE})
    target_include_directories(${BENCHMARK_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    set_target_properties(${BENCHMARK_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED TRUE
    )
    target_link_libraries(${BENCHMARK_NAME}
        ${PROJECT_NAME}
        nanobench
    )
endfunction()

add_benchmark(non_null-optional optional/non_null.cpp)
add_benchmark(non_null-throwing throwing/non_null.cpp)

add_benchmark(bound_check-optional optional/bound_check.cpp)
add_benchmark(bound_check-throwing throwing/bound_check.cpp)
