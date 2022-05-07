#include "swf/swf.hpp"
#include "modules.hpp"
#include <stdexcept>

void init_swf(py::module& m) {
    py::enum_<swf::Compression>(m, "Compression")
        .value("None", swf::Compression::None, "no compression")
        .value("Zlib", swf::Compression::Zlib, "zlib compression")
        .value("Lzma", swf::Compression::Lzma, "lzma compression");

    // define it after swf::Compression has been defined to prevent C++ types in docstrings
    m.def("isValidCompression", py::overload_cast<swf::Compression>(&swf::isValidCompression),
        "Check if a compression is valid and supported.", "compression"_a);

    py::class_<swf::Swf>(m, "Swf", "Represent a ShockWave Flash file.")
        .def(py::init())
        .def("read", &swf::Swf::read, "stream"_a)
        .def("write", &swf::Swf::write, "stream"_a)
        .def_property_readonly("compressed", &swf::Swf::isCompressed, "Returns wether or not the file is compressed.")
        .def_property(
            "compression", [](const swf::Swf& s) { return static_cast<char>(s.signature.data()[0]); },
            [](swf::Swf& s, const char c) {
                if (!swf::isValidCompression(c))
                    throw std::invalid_argument("Invalid compression.");

                s.signature[0] = c;
            })
        .def_property(
            "signature",
            [](const swf::Swf& s) {
                return std::string(reinterpret_cast<const char*>(s.signature.data()), s.signature.size());
            },
            [](swf::Swf& s, std::string& sig) {
                if (sig.size() != 3)
                    throw std::length_error("A valid signature must be 3 char long.");

                if (!swf::isValidCompression(sig[0]))
                    throw std::invalid_argument("Invalid compression.");

                if (sig[1] != 'W' || sig[2] != 'S')
                    throw std::invalid_argument("Invalid signature.");

                s.signature[0] = sig[0];
            },
            "SWF's signature. The first character must be a valid compression. The two others must be 'W' and 'S'.")
        .def_readwrite("version", &swf::Swf::version, "SWF's version")
        .def_readwrite("tags", &swf::Swf::tags, "Contains all tags")
        .def_readwrite("symbol_class", &swf::Swf::symbol_class)
        .def_readwrite("framesize", &swf::Swf::framesize)
        .def_readwrite("framerate", &swf::Swf::framerate, "SWF's framerate")
        .def_readwrite("framecount", &swf::Swf::framecount, "The total number of frames")
        .def_readwrite("binaries", &swf::Swf::binaries, "List of all binary data")
        .def_readwrite("abcfiles", &swf::Swf::abcfiles, "Map of all abc tags")
        .def_readonly("file_length", &swf::Swf::file_length, "The (uncompressed) file's total length");
}