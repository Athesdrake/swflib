#include "abc/AbcFile.hpp"

namespace swf::abc {
AbcFile::AbcFile() {
    minor_version = 16;
    major_version = 46;

    methods = std::vector<Method>();
    classes = std::vector<Class>();
    scripts = std::vector<Script>();
}

AbcFile::AbcFile(StreamReader& stream) { read(stream); }
AbcFile::AbcFile(StreamReader&& stream) { read(stream); }

void AbcFile::read_methods(StreamReader& stream) {
    uint32_t count = stream.readU30();
    methods.resize(count);
    for (unsigned i = 0; i < count; i++) {
        methods[i] = Method(this);
        methods[i].read(stream);
    }
}

void AbcFile::read_bodies(StreamReader& stream) {
    uint32_t count = stream.readU30();
    for (unsigned i = 0; i < count; i++) {
        methods[stream.readU30()].read_body(stream);
    }
}

std::vector<uint32_t> AbcFile::write_methods(StreamWriter& stream) {
    uint32_t count = (uint32_t)methods.size();
    std::vector<uint32_t> bodies;

    stream.writeU30(count);
    for (uint32_t i = 0; i < count; i++) {
        methods[i].write(stream);
        if (methods[i].has_body())
            bodies.push_back(i);
    }

    return bodies;
}

void AbcFile::write_bodies(StreamWriter& stream, std::vector<uint32_t> bodies) {
    stream.writeU30((uint32_t)bodies.size());
    for (auto i : bodies) {
        stream.writeU30(i);
        methods[i].write_body(stream);
    }
}

void AbcFile::write(StreamWriter& stream) {
    uint32_t count;

    stream.writeU16(minor_version);
    stream.writeU16(major_version);

    cpool.write(stream);

    auto bodies = write_methods(stream);

    stream.writeU30(0); // metadatas

    count = (uint32_t)classes.size();
    stream.writeU30(count);
    for (unsigned i = 0; i < count; i++)
        classes[i].write_instance(stream);

    for (unsigned i = 0; i < count; i++)
        classes[i].write(stream);

    count = (uint32_t)scripts.size();
    stream.writeU30(count);
    for (unsigned i = 0; i < count; i++)
        scripts[i].write(stream);

    write_bodies(stream, bodies);
}

void AbcFile::read(StreamReader& stream) {
    uint32_t count, count2;

    minor_version = stream.readU16();
    major_version = stream.readU16();

    cpool.read(stream);
    read_methods(stream);

    count = stream.readU30();
    // skip metadate, maybe add support in the future?
    for (unsigned i = 0; i < count; i++) {
        stream.skip30();
        count2 = stream.readU30();
        for (unsigned j = 0; j < count2; j++)
            stream.skip30(2);
    }

    count = stream.readU30();
    classes.resize(count);
    for (unsigned i = 0; i < count; i++) {
        classes[i] = Class(this);
        classes[i].read_instance(stream);
    }

    for (unsigned i = 0; i < count; i++)
        classes[i].read(stream);

    count = stream.readU30();
    scripts.resize(count);
    for (unsigned i = 0; i < count; i++) {
        scripts[i] = Script(this);
        scripts[i].read(stream);
    }

    read_bodies(stream);
}

// std::vector<Namespace> AbcFile::get_namespace_set(uint32_t index) {
//     auto nss = cpool.ns_sets[index];
//     std::vector<Namespace> resolved(nss.size());

//     for (auto i = 0; i < nss.size(); ++i)
//         resolved[i] = cpool.namespaces[nss[i]];

//     return resolved;
// }
}