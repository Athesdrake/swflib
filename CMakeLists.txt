cmake_minimum_required(VERSION 3.13)
file(READ "${PROJECT_SOURCE_DIRECTORY}include/swflib.hpp" _SWFLIB)
string(REGEX MATCH "version[ \t]+=[ \t]+\"([^\"]+)\"" _ ${_SWFLIB})
set(VERSION ${CMAKE_MATCH_1})
message(STATUS "swflib version: ${VERSION}")

project(
  swflib
  LANGUAGES CXX
  VERSION ${VERSION})
set(CMAKE_CXX_STANDARD 17)

add_library(swflib SHARED "")
add_subdirectory(lib)

# Remove the "lib" prefix
set_target_properties(swflib PROPERTIES PREFIX "")

# Add the header files
target_include_directories(
  swflib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                $<INSTALL_INTERFACE:include>)

# Find & link dependencies
find_package(ZLIB REQUIRED)
find_package(LibLZMA REQUIRED)
target_link_libraries(swflib PRIVATE ZLIB::ZLIB)
target_link_libraries(swflib PRIVATE LibLZMA::LibLZMA)

# Install stuff
export(TARGETS swflib FILE ${PROJECT_BINARY_DIR}/swflibTargets.cmake)
export(PACKAGE swflib)

file(RELATIVE_PATH REL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/lib/CMake/swflib
     ${CMAKE_INSTALL_PREFIX}/include)
set(swflib_INCLUDE_DIRS ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR})
configure_file(cmake/swflibConfig.cmake.in
               ${PROJECT_BINARY_DIR}/swflibConfig.cmake @ONLY)
set(swflib_INCLUDE_DIRS "\${SWFLIB_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(
  cmake/swflibConfig.cmake.in
  ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/swflibConfig.cmake @ONLY)

install(
  TARGETS swflib
  EXPORT swflibTargets
  RUNTIME DESTINATION bin COMPONENT bin
  LIBRARY DESTINATION lib COMPONENT shlib
  PUBLIC_HEADER DESTINATION include/swflib COMPONENT dev)
install(
  FILES ${PROJECT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/swflibConfig.cmake
  DESTINATION lib/CMake/swflib
  COMPONENT dev)
install(
  EXPORT swflibTargets
  DESTINATION lib/CMake/swflib
  COMPONENT dev)
install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})

# Only include tests if this is the main project. You can override this behavior
# by setting SWFLIB_CMAKE_BUILD_TESTING to true
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR SWFLIB_CMAKE_BUILD_TESTING)
   AND BUILD_TESTING)
  include(CTest)
  add_subdirectory(tests)
endif()
