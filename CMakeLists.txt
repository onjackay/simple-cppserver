# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(MyProject VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(src/include)

# Source files
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/include/*.h)

# Add the executable for each source file in test/
file(GLOB TEST_SOURCES test/*.cpp)
foreach(TEST_SRC ${TEST_SOURCES})
    # Get the file name without directory and extension
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    # Add executable
    add_executable(${TEST_NAME} ${TEST_SRC} ${SOURCES} ${HEADERS})
    # Link libraries or dependencies if needed
    # target_link_libraries(${TEST_NAME} <libraries>)
endforeach()
