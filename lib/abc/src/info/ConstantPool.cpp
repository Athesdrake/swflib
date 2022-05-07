#include "abc/info/ConstantPool.hpp"
#include "abc/AbcFile.hpp"
#include <cmath>

namespace swf::abc {
ConstantPool::ConstantPool() {
    integers   = std::vector<int32_t>();
    uintegers  = std::vector<uint32_t>();
    doubles    = std::vector<double>();
    strings    = std::vector<std::string>();
    namespaces = std::vector<Namespace>();
    ns_sets    = std::vector<std::vector<uint32_t>>();
    multinames = std::vector<Multiname>();
}

void ConstantPool::read(StreamReader& stream) {
    uint32_t count;

    count = stream.readU30();
    integers.resize(std::max(1u, count));
    integers[0] = 0;
    for (unsigned i = 1; i < count; i++)
        integers[i] = stream.readS30();

    count = stream.readU30();
    uintegers.resize(std::max(1u, count));
    uintegers[0] = 0;
    for (unsigned i = 1; i < count; i++)
        uintegers[i] = stream.readU30();

    count = stream.readU30();
    doubles.resize(std::max(1u, count));
    doubles[0] = 0;
    for (unsigned i = 1; i < count; i++)
        doubles[i] = stream.readDouble();

    count = stream.readU30();
    strings.resize(std::max(1u, count));
    strings[0] = "";
    for (unsigned i = 1; i < count; i++)
        strings[i] = stream.readStringView();

    count = stream.readU30();
    namespaces.resize(std::max(1u, count));
    namespaces[0] = Namespace();
    for (unsigned i = 1; i < count; i++)
        namespaces[i].read(stream);

    count = stream.readU30();
    ns_sets.resize(std::max(1u, count));
    for (unsigned i = 1; i < count; i++) {
        uint32_t ns_count = stream.readU30();

        ns_sets[i] = std::vector<uint32_t>(ns_count);
        for (unsigned j = 0; j < ns_count; j++) {
            ns_sets[i][j] = stream.readU30();
        }
    }

    count = stream.readU30();
    multinames.resize(std::max(1u, count));
    multinames[0] = Multiname();
    for (unsigned i = 1; i < count; i++)
        multinames[i].read(stream);
}

void ConstantPool::write(StreamWriter& stream) {
    uint32_t count = (uint32_t)integers.size();
    stream.writeU30(count > 1 ? count : 0);
    for (unsigned i = 1; i < count; i++)
        stream.writeS30(integers[i]);

    count = (uint32_t)uintegers.size();
    stream.writeU30(count > 1 ? count : 0);
    for (unsigned i = 1; i < count; i++)
        stream.writeU30(uintegers[i]);

    count = (uint32_t)doubles.size();
    stream.writeU30(count > 1 ? count : 0);
    for (unsigned i = 1; i < count; i++)
        stream.writeDouble(doubles[i]);

    count = (uint32_t)strings.size();
    stream.writeU30(count > 1 ? count : 0);
    for (unsigned i = 1; i < count; i++)
        stream.writeStringView(strings[i]);

    count = (uint32_t)namespaces.size();
    stream.writeU30(count > 1 ? count : 0);
    for (unsigned i = 1; i < count; i++)
        namespaces[i].write(stream);

    count = (uint32_t)ns_sets.size();
    stream.writeU30(count > 1 ? count : 0);
    for (unsigned i = 1; i < count; i++) {
        uint32_t ns_count = (uint32_t)ns_sets[i].size();
        stream.writeU30(ns_count);

        for (unsigned j = 0; j < ns_count; j++) {
            stream.writeU30(ns_sets[i][j]);
        }
    }

    count = (uint32_t)multinames.size();
    stream.writeU30(count > 1 ? count : 0);
    for (unsigned i = 1; i < count; i++)
        multinames[i].write(stream);
}
}