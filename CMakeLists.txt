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
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/src/main.cpp
    
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/src/value_pack/value_pack.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/src/value_pack/concat.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/src/value_pack/to.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/src/value_pack/fold.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/src/value_pack/contains.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/src/value_pack/filter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/src/value_pack/diff.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/src/value_pack/unique.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/tests/src/optimizer/optimizer.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/tests/src/constrained_type.cpp
)

target_include_directories(${PROJECT_NAME}-tests PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/include
)

set_target_properties(${PROJECT_NAME}-tests PROPERTIES
    CXX_STANDARD          20
    CXX_STANDARD_REQUIRED TRUE
)

target_link_libraries(${PROJECT_NAME}-tests ${PROJECT_NAME})
