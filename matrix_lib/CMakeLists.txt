cmake_minimum_required(VERSION 3.10)
project(MatriXLib VERSION 1.0.0 DESCRIPTION "The MatriX library!")
# Header only library
add_library(MatriXLib INTERFACE)
target_include_directories(MatriXLib
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(BUILD_TESTS True)

if(BUILD_TESTS)
    
    set(TEST_MAIN tests.cpp)
    set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)
    set(TESTS 
        ${TEST_DIR}/matrix_test.cpp
        ${TEST_DIR}/rotmat_test.cpp
        ${TEST_DIR}/rotvec_test.cpp
        ${TEST_DIR}/vector_test.cpp
        )
    # Executable for tests, linking to library
    add_executable(MatriXLibTest ${TEST_DIR}/${TEST_MAIN} ${TESTS})
    target_link_libraries(MatriXLibTest MatriXLib)

    # Locate GTest
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})

    target_link_libraries(MatriXLibTest ${GTEST_LIBRARIES} pthread)
    # Link the test executable with Google Test
    target_link_libraries(MatriXLibTest gtest_main)
    
    # Add the test executable to CTest
    include(GoogleTest)
    gtest_discover_tests(MatriXLibTest)
endif()