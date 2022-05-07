add_subdirectory(swf)
add_subdirectory(abc)
add_subdirectory(py)
target_sources(swflib PRIVATE bitstream.cpp stream.cpp)
