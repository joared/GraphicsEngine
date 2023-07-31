cmake_minimum_required(VERSION 3.10)

# Set the compiler to g++
set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_STANDARD 17)

# Set the build type (default to "Release" if not specified)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Set the flags for the compiler
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3")
# add any other flags as needed

# Define the project. Can be used to modify the behaviour of the build.
project(GraphicsEngine VERSION 1.0.0 DESCRIPTION "The Graphics Engine!")

# Define the path to the main source file
set(SRC_MAIN main.cpp)

# Define the path to the source directory
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# The source files needed to compile the project executable
set(SOURCES  
    ${SRC_DIR}/camera.cpp
    ${SRC_DIR}/object.cpp
    ${SRC_DIR}/engine.cpp
    ${SRC_DIR}/particle_sim.cpp
)

# Add the executable
# add_executable(GraphicsEngine ${SRC_DIR}/${SRC_MAIN} ${SOURCES})
# target_include_directories(GraphicsEngine
#   PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
# )
# target_link_libraries(GraphicsEngine PRIVATE MatriXLib CImgProject)

add_executable(RobotArm ${SRC_DIR}/robot_arm_sim.cpp ${SOURCES})
target_include_directories(RobotArm
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(RobotArm PRIVATE MatriXLib CImgProject)

# Add a library to easy link test cases
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/matrix_lib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/cimg_lib)
# Link against the MatriXLib library


# Simple CImg compile test
add_executable(FireWorks ${SRC_DIR}/fireworks.cpp)
target_link_libraries(FireWorks PRIVATE CImgProject)

# # Check if the BUILD_TESTS flag is set
# set(BUILD_TESTS True)

# if(BUILD_TESTS)
#     # This is needed to install the test executable (dynamic linking)
#     set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

#     set(TEST_MAIN tests.cpp)
#     set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)
#     set(TESTS 
#         ${TEST_DIR}/veclib_test.cpp
#         )
#     # Executable for tests, linking to library
#     add_executable(GraphicsEngineTest ${TEST_DIR}/${TEST_MAIN} ${TESTS})
#     target_link_libraries(GraphicsEngineTest GraphicsEngineLib)

#     # Locate GTest
#     find_package(GTest REQUIRED)
#     include_directories(${GTEST_INCLUDE_DIRS})

#     target_link_libraries(GraphicsEngineTest ${GTEST_LIBRARIES} pthread)
#     # Link the test executable with Google Test
#     target_link_libraries(GraphicsEngineTest gtest_main)
    
#     # Add the test executable to CTest
#     include(GoogleTest)
#     gtest_discover_tests(GraphicsEngineTest)
# endif()
