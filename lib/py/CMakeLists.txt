find_package(pybind11 REQUIRED)
pybind11_add_module(pyswflib src/abc.cpp src/swflib.cpp src/stream.cpp
                    src/swf.cpp src/tags.cpp)

set_target_properties(pyswflib PROPERTIES OUTPUT_NAME "_swflib")
target_include_directories(pyswflib PRIVATE ${CMAKE_CURRENT_LIST_DIR}/inc)
target_link_libraries(pyswflib PRIVATE swflib)
