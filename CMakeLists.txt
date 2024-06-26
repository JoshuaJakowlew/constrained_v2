cmake_minimum_required(VERSION 3.22)

project(constrained_type
    VERSION 0.0.0
    LANGUAGES CXX
)

add_library(${PROJECT_NAME} INTERFACE)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD          20
    CXX_STANDARD_REQUIRED TRUE
)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# add_executable(${PROJECT_NAME}-bin
#     src/main.cpp
# )

# set_target_properties(${PROJECT_NAME}-bin PROPERTIES
#     CXX_STANDARD          20
#     CXX_STANDARD_REQUIRED TRUE
# )

# target_link_libraries(${PROJECT_NAME}-bin ${PROJECT_NAME})

add_executable(${PROJECT_NAME}-tests
    tests/main.cpp
    
    tests/value_pack/value_pack.cpp
    tests/value_pack/concat.cpp
    tests/value_pack/to.cpp
    tests/value_pack/fold.cpp
    tests/value_pack/contains.cpp
    tests/value_pack/filter.cpp
    tests/value_pack/diff.cpp
    tests/value_pack/unique.cpp

    tests/optimizer/optimizer.cpp
)

set_target_properties(${PROJECT_NAME}-tests PROPERTIES
    CXX_STANDARD          20
    CXX_STANDARD_REQUIRED TRUE
)

target_link_libraries(${PROJECT_NAME}-tests ${PROJECT_NAME})
