cmake_minimum_required(VERSION 3.13)
project(swflib CXX)

set(CMAKE_CXX_STANDARD 17)
include(CTest)
enable_testing()

include_directories(include)
add_library(swflib SHARED "")
add_subdirectory(lib)
add_subdirectory(tests)

file(READ "include/swflib.h" _SWFLIB)
string(REGEX MATCH "version[ \t]+=[ \t]+\"([^\"]+)\"" _ ${_SWFLIB})
set(VERSION ${CMAKE_MATCH_1})
message(STATUS "swflib version: ${VERSION}")

find_package(ZLIB REQUIRED)
find_package(LibLZMA REQUIRED)
target_link_libraries(swflib PRIVATE ZLIB::ZLIB)
target_link_libraries(swflib PRIVATE LibLZMA::LibLZMA)
