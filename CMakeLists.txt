# CMake Specification
cmake_minimum_required(VERSION 3.10)

# Project Details
project(stereo3D)
set(CMAKE_CXX_COMPILER g++-9)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 1)

# Set Debug if nothing specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

# Enable code coverage for GNU compiler in Debug mode
if(CMAKE_BUILD_TYPE MATCHES Debug AND CMAKE_COMPILER_IS_GNUCXX)
    include(ProcessorCount)
    ProcessorCount(N)
    if(NOT N EQUAL 0)
        set(CMAKE_CTEST_COMMAND "${CMAKE_CTEST_COMMAND} -j${N}")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage -lgcov")
endif()

# Setup OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Setup GoogleTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Threads
find_package(Threads REQUIRED)

# Executables
add_executable(calibration.bin src/main.cpp src/calibration.cpp src/camera.cpp)
add_executable(showImg.bin src/showImg_main.cpp src/showImg.cpp)

# Link libraries
target_link_libraries(showImg.bin ${OpenCV_LIBS})
target_link_libraries(calibration.bin ${OpenCV_LIBS} Threads::Threads)

# Testing
add_executable(runTests.bin tests/main_tests.cpp tests/camera_test.cpp tests/showImg_test.cpp src/showImg.cpp src/camera.cpp)

# Link testing libraries
target_link_libraries(runTests.bin ${OpenCV_LIBS} ${GTEST_LIBRARIES} pthread)
