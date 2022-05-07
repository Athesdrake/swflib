function(test filename)
  string(REGEX REPLACE ".cpp$" "" name_test ${filename})
  string(REGEX REPLACE "^test_" "" name ${name_test})
  add_executable(${name_test} ${filename})
  target_link_libraries(${name_test} swflib)
  add_test(${name} ${name_test} ${ARGN})

  foreach(arg IN LISTS ARGN)
    message(STATUS "Adding file ${arg} to the target ${name_test}.")
    add_custom_command(
      TARGET ${name_test}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${arg}"
              "$<TARGET_FILE_DIR:${name_test}>/${arg}")
  endforeach()
endfunction(test)

test(test_bitstream.cpp)
test(test_stream.cpp)
test(test_swf.cpp 1.swf 2.swf)
test(test_abc.cpp 1.swf 2.swf)
test(test_abcexport.cpp 1.swf 2.swf)
test(test_parser.cpp 1.swf 2.swf)
