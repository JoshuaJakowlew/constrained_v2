include(FetchContent)

FetchContent_Declare(
    doctest
    GIT_REPOSITORY "https://github.com/onqtam/doctest"
    GIT_TAG "master"
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(doctest)

add_executable(${PROJECT_NAME}-tests
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    
    ${CMAKE_CURRENT_SOURCE_DIR}/src/value_pack/value_pack.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/value_pack/concat.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/value_pack/to.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/value_pack/fold.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/value_pack/contains.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/value_pack/filter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/value_pack/diff.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/value_pack/unique.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/optimizer/optimizer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/optimizer/constraints.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/optimizer/fail_handlers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/optimizer/validators.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/constrained_type.cpp
)

target_include_directories(${PROJECT_NAME}-tests PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(${PROJECT_NAME}-tests PROPERTIES
    CXX_STANDARD          20
    CXX_STANDARD_REQUIRED TRUE
)

target_link_libraries(${PROJECT_NAME}-tests
    ${PROJECT_NAME}
    doctest
)