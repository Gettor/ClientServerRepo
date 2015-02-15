cmake_minimum_required(VERSION 2.8)

project("ClientServer")


# variables needed to find Boost and GTest
set(BOOST_ROOT "/usr")
set(GTEST_ROOT "/usr")

find_package(Boost 1.57)
if(Boost_FOUND)
    include_directories(${Booat_INCLUDE_DIRS})
else()
    message(FATAL_ERROR, " Boost not found or it is too old")
endif()

find_package(GTest REQUIRED)
if(GTEST_FOUND)
    include_directories(${GTEST_INCLUDE_DIRS})
else()
    message(FATAL_ERROR, " GTest not found")
endif()

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_CXX_FLAGS "-Wall -std=c++11 -pthread")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories("common/external/cereal/include")

add_subdirectory(client)
add_subdirectory(common)
# add_subdirectory(server)


enable_testing()

# add test binaries here
add_test(clientTest bin/clientTest)