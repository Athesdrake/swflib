#include "modules.h"

PYBIND11_MODULE(_swflib, m) {
    m.doc()     = "A swf parser made in c++";
    auto swf    = m.def_submodule("swf", "Library related to swf files and stuff.");
    auto tags   = swf.def_submodule("tags", "Definition of swf tags.");
    auto abc    = m.def_submodule("abc", "Library related to ActionScript ByteCode files and stuff.");
    auto stream = m.def_submodule("stream", "Library related to streams used by swflib.");

    py::class_<abc::AbcFile> abcfile(m, "AbcFile",
        "Represent an ABC file which contains compiled programs: constant data, instructions and various kinds of "
        "metdata");

    init_stream(stream);
    init_tags(tags);
    init_swf(swf);
    init_abc(abc, abcfile);

    // Bind opaque types
    py::bind_map<std::unordered_map<std::string, swf::DoABCTag*>>(m, "MapAbcTag");
    py::bind_map<std::unordered_map<uint16_t, std::string>>(m, "MapCharacter");
    py::bind_vector<std::vector<abc::Class>>(m, "VectorClass");
    py::bind_vector<std::vector<abc::Exception>>(m, "VectorException");
    py::bind_vector<std::vector<abc::Method>>(m, "VectorMethod");
    py::bind_vector<std::vector<abc::Multiname>>(m, "VectorMultiname");
    py::bind_vector<std::vector<abc::Namespace>>(m, "VectorNamespace");
    py::bind_vector<std::vector<abc::Option>>(m, "VectorOption");
    py::bind_vector<std::vector<abc::Script>>(m, "VectorScript");
    py::bind_vector<std::vector<abc::Trait>>(m, "VectorTrait");
    py::bind_vector<std::vector<double>>(m, "VectorDouble");
    py::bind_vector<std::vector<int32_t>>(m, "VectorInt");
    py::bind_vector<std::vector<std::shared_ptr<swf::Tag>>>(m, "VectorTag");
    py::bind_vector<std::vector<std::string>>(m, "VectorString");
    py::bind_vector<std::vector<swf::DefineBinaryDataTag*>>(m, "VectorBinary");
    py::bind_vector<std::vector<uint32_t>>(m, "VectorUInt");
    py::bind_vector<std::vector<uint8_t>>(m, "VectorByte");
    // Bind after the inner vector is registered
    py::bind_vector<std::vector<std::vector<uint32_t>>>(m, "VectorNsSet");
}
