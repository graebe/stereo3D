# CMake Specification
cmake_minimum_required(VERSION 3.10)

# Project Details
project(stereo3D)
set(CMAKE_CXX_STANDARD 17)

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
add_executable(calibration.so src/main.cpp src/calibration.cpp src/camera.cpp)
add_executable(showImg.so src/showImg_main.cpp src/showImg.cpp)
add_executable(capture.so src/capture.cpp)

# Link libraries
target_link_libraries(showImg.so ${OpenCV_LIBS})
target_link_libraries(calibration.so ${OpenCV_LIBS} Threads::Threads)
target_link_libraries(capture.so ${OpenCV_LIBS})

# Testing
add_executable(runTests.so tests/showImg_test.cpp src/showImg.cpp)

# Link testing libraries
target_link_libraries(runTests.so ${OpenCV_LIBS} ${GTEST_LIBRARIES} pthread)
