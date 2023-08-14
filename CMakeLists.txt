# CMake Specification
cmake_minimum_required(VERSION 3.10)

# Project Details
project(stereo3D)
set(CMAKE_CXX_STANDARD 17)

# Setup OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Setup GoogleTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Executables
add_executable(HelloWorld src/main.cpp)
add_executable(showImg.so src/showImg_main.cpp src/showImg.cpp)

# Link libraries
target_link_libraries(showImg.so ${OpenCV_LIBS})

# Uncomment below when you're ready to setup tests
# Testing
add_executable(runTests.so tests/showImg_test.cpp src/showImg.cpp)
target_link_libraries(runTests.so ${OpenCV_LIBS} ${GTEST_LIBRARIES} pthread)
