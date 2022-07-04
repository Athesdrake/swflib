option(PY_EXT "Enable building Python extension using pybind11." ON)

add_subdirectory(swf)
add_subdirectory(abc)
if(${PY_EXT})
  add_subdirectory(py)
endif()
target_sources(swflib PRIVATE bitstream.cpp stream.cpp)
