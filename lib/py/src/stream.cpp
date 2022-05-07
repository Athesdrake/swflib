#include "modules.hpp"

void init_stream(py::module& m) {
    py::class_<swf::StreamReader>(m, "StreamReader",
        "Despite its name, it is not a stream. It holds a binary data in memory and provides methods to read and "
        "extract values from its buffer.The dtor will delete the buffer, if marked as managed.")
        .def(py::init())
        .def(py::init([](py::buffer const data) {
            auto info = data.request();
            auto ptr  = static_cast<uint8_t*>(info.ptr);
            if (info.format != py::format_descriptor<uint8_t>::format() || info.ndim != 1)
                throw std::runtime_error("Incompatible buffer format!");

            return new swf::StreamReader(ptr, ptr + info.size, false);
        }),
            "Creates a new StreamReader from bytes.", "data"_a, py::keep_alive<1, 2>())
        .def(py::init(&swf::StreamReader::fromfile), "Creates a new StreamReader from a file.", "filename"_a)
        .def("skip30", &swf::StreamReader::skip30, "Skip n variable-length int(s).")
        .def("skip", &swf::StreamReader::skip, "Skip n bytes.")
        .def("reset", &swf::StreamReader::reset, "Reset the position to the start of the buffer.")
        .def("readU8", &swf::StreamReader::readU8, "Read an unsigned 8-bit integer")
        .def("readU64", &swf::StreamReader::readU64, "Read an unsigned 64-bit integer")
        .def("readU32", &swf::StreamReader::readU32, "Read an unsigned 32-bit integer")
        .def("readU30", &swf::StreamReader::readU30,
            "Read a variable-length unsigned integer. See https://en.wikipedia.org/wiki/LEB128 for more informations.")
        .def("readU24", &swf::StreamReader::readU24, "Read an unsigned 24-bit integer")
        .def("readU16", &swf::StreamReader::readU16, "Read an unsigned 16-bit integer")
        .def("readStringView", &swf::StreamReader::readStringView,
            "Same as readString but return a string_view and therefore uses less memory.")
        .def("readString", &swf::StreamReader::readString,
            "Read a string preceded with a U30 integer defining the string's length.")
        .def("readS8", &swf::StreamReader::readS8, "Read a signed 8-bit integer")
        .def("readS64", &swf::StreamReader::readS64, "Read a signed 64-bit integer")
        .def("readS32", &swf::StreamReader::readS32, "Read a signed 32-bit integer")
        .def("readS30", &swf::StreamReader::readS30,
            "Read a variable-length signed integer. See https://en.wikipedia.org/wiki/LEB128 for more informations.")
        .def("readS24", &swf::StreamReader::readS24, "Read a signed 24-bit integer")
        .def("readS16", &swf::StreamReader::readS16, "Read a signed 16-bit integer")
        .def("readFloat", &swf::StreamReader::readFloat,
            "Read a 32-bit IEEE 754 single-precision binary floating-point.")
        .def("readDouble", &swf::StreamReader::readDouble,
            "Read a 64-bit IEEE 754 double-precision binary floating-point.")
        .def("readCharArray", &swf::StreamReader::readCharArray, "Read a null-terminated char array.")
        .def(
            "readBytes",
            [](swf::StreamReader& stream, uint32_t length) {
                std::string str;
                stream.copy(length, str);
                return py::bytes(str);
            },
            "Read n bytes from the buffer.")
        .def("inflateZlib", &swf::StreamReader::inflateZlib,
            "Inflate the next n bytes using zlib. The internal buffer will be deleted (if managed) and replaced by the "
            "inflated data. Hence the data beyond these n bytes will be lost.")
        .def("inflateLzma", &swf::StreamReader::inflateLzma, "Same as inflateZlib but uses LZMA.")
        .def("__len__", &swf::StreamReader::size, "Return the total size of the buffer.")
        .def("__bool__", &swf::StreamReader::operator bool, "Return false if the current position is out of bound.")
        .def_property_readonly(
            "valid", &swf::StreamReader::valid, "Return false if the current position is out of bound.")
        .def_property_readonly("remaining", &swf::StreamReader::remaining, "Return the remaining bytes in the buffer.")
        .def_property_readonly("end", &swf::StreamReader::end, "Tells if the end of the buffer has been reached.");

    py::class_<swf::StreamWriter>(m, "StreamWriter",
        "Despite its name, it is not a stream. It holds a binary data in memory and provides methods to write and "
        "insert values into its buffer. The dtor will delete the internal buffer.")
        .def(py::init(), "Creates a new StreamWriter.")
        .def("writeU8", &swf::StreamWriter::writeU8, "Read an unsigned 8-bit integer")
        .def("writeU64", &swf::StreamWriter::writeU64, "Write an unsigned 64-bit integer")
        .def("writeU32", &swf::StreamWriter::writeU32, "Write an unsigned 32-bit integer")
        .def("writeU30", &swf::StreamWriter::writeU30,
            "Write a variable-length unsigned integer. See https://en.wikipedia.org/wiki/LEB128 for more informations.")
        .def("writeU24", &swf::StreamWriter::writeU24, "Write an unsigned 24-bit integer")
        .def("writeU16", &swf::StreamWriter::writeU16, "Write an unsigned 16-bit integer")
        .def("writeStringView", &swf::StreamWriter::writeStringView,
            "Write as readString but return a string_view and therefore uses less memory.")
        .def("writeString", &swf::StreamWriter::writeString,
            "Write a string preceded with a U30 integer defining the string's length.")
        .def("writeS8", &swf::StreamWriter::writeS8, "Write a signed 8-bit integer")
        .def("writeS64", &swf::StreamWriter::writeS64, "Write a signed 64-bit integer")
        .def("writeS32", &swf::StreamWriter::writeS32, "Write a signed 32-bit integer")
        .def("writeS30", &swf::StreamWriter::writeS30,
            "Write a variable-length signed integer. See https://en.wikipedia.org/wiki/LEB128 for more informations.")
        .def("writeS24", &swf::StreamWriter::writeS24, "Write a signed 24-bit integer")
        .def("writeS16", &swf::StreamWriter::writeS16, "Write a signed 16-bit integer")
        .def("writeFloat", &swf::StreamWriter::writeFloat,
            "Write a 32-bit IEEE 754 single-precision binary floating-point.")
        .def("writeDouble", &swf::StreamWriter::writeDouble,
            "Write a 64-bit IEEE 754 double-precision binary floating-point.")
        .def("writeCharArray", &swf::StreamWriter::writeCharArray, "Write a null-terminated char array.")
        // Unable to link against py::overload_cast<swf::StreamReader&>(&swf::StreamWriter::write)
        // This fixes the problem:
        .def(
            "write", [](swf::StreamWriter& stream, swf::StreamWriter& s) { stream.write(s); },
            "Write the buffer of another stream into its own buffer.")
        .def(
            "write", [](swf::StreamWriter& stream, swf::StreamReader& s) { stream.write(s); },
            "Write the buffer of another stream into its own buffer.")
        .def("tofile", py::overload_cast<std::string>(&swf::StreamWriter::tofile),
            "Write the buffer to a file. It supports windows API to write data faster.")
        .def(
            "extend",
            [](swf::StreamWriter& stream, py::buffer data) {
                stream.extend(py::len(data), reinterpret_cast<uint8_t*>(data.request().ptr));
            },
            "Copy n bytes into the internal buffer from a bytes object.")
        .def("deflateZlibInto", &swf::StreamWriter::deflateZlibInto,
            "Deflate the internal buffer into the given stream using zlib.")
        .def("deflateZlib", &swf::StreamWriter::deflateZlib,
            "Deflate the internal buffer into the given buffer using zlib.")
        .def("deflateLzmaInto", &swf::StreamWriter::deflateLzmaInto,
            "eflate the internal buffer into the given stream using LZMA.")
        .def("deflateLzma", &swf::StreamWriter::deflateLzma,
            "Deflate the internal buffer into the given buffer using LZMA.")
        .def("__len__", &swf::StreamWriter::size, "Return the current size of the buffer.");

    py::class_<swf::BitStreamReader>(m, "BitStreamReader", "Utility class to read bits from a StreamReader.")
        .def(py::init<swf::StreamReader*>())
        .def("readUB", &swf::BitStreamReader::readUB, "Read n bits and return it as an unsigned integer.")
        .def("readSB", &swf::BitStreamReader::readSB, "Read n bits and return it as a signed integer.");

    py::class_<swf::BitStreamWriter>(m, "BitStreamWriter", "Utility class to write bits into a StreamWrite.")
        .def(py::init<swf::StreamWriter*>())
        .def("writeUB", &swf::BitStreamWriter::writeUB, "Write an unsigned integer into n bits.")
        .def("writeSB", &swf::BitStreamWriter::writeSB, "Write a signed integer into n bits.");
    // .def_static(
    //     "calcBits",
    //     [](py::int_ nbr, bool is_signed) {
    //         if (is_signed)
    //             return swf::BitStreamWriter::calcBits(nbr.cast<int32_t>());
    //         return swf::BitStreamWriter::calcBits(nbr.cast<uint32_t>());
    //     },
    //     py::arg("is_signed") = false);

    m.def(
        "calcBits",
        [](py::int_ nbr, bool is_signed) {
            if (is_signed)
                return swf::BitStreamWriter::calcBits(nbr.cast<int32_t>());
            return swf::BitStreamWriter::calcBits(nbr.cast<uint32_t>());
        },
        "Compute the number of bits required to write an integer.", "nbr"_a, "is_signed"_a = false);
}