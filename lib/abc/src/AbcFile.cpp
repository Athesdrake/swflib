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

std::string& AbcFile::qname(uint32_t& index) { return qname(cpool.multinames[index]); }
std::string& AbcFile::qname(Multiname& mn) { return cpool.strings[mn.get_name_index()]; }

std::string AbcFile::str(std::vector<uint32_t>& ns_set) {
    std::string name = "";
    for (auto& index : ns_set) {
        if (!name.empty())
            name += "::";
        name += str(cpool.namespaces[index]);
    }
    return name;
}

std::string& AbcFile::str(Namespace& ns) { return cpool.strings[ns.name]; }
std::string AbcFile::str(uint32_t& index) { return str(cpool.multinames[index]); }
std::string AbcFile::str(Multiname& mn) {
    if (mn.kind == MultinameKind::Typename) {
        std::string types;
        if (!mn.types.empty()) {
            for (auto& type : mn.types)
                types += ',' + (type == 0 ? "*" : str(type));

            types.front() = '<';
            types += '>';
        }

        return str(mn.data.type_name.qname) + types;
    }

    if (mn.kind == MultinameKind::MultinameL || mn.kind == MultinameKind::MultinameLA)
        return str(cpool.ns_sets[mn.data.multiname_l.ns_set]);

    return qname(mn);
}

std::string AbcFile::ns(Multiname& mn) {
    switch (mn.kind) {
    case MultinameKind::QName:
    case MultinameKind::QNameA:
        if (mn.data.qname.ns == 0)
            return "";

        return str(cpool.namespaces[mn.data.qname.ns]);
    case MultinameKind::RTQName:
    case MultinameKind::RTQNameA:
    case MultinameKind::RTQNameL:
    case MultinameKind::RTQNameLA:
        // RTQName does not have namespaces
        return "";
    case MultinameKind::Multiname:
    case MultinameKind::MultinameA:
    case MultinameKind::MultinameL:
    case MultinameKind::MultinameLA:
        return str(cpool.ns_sets[mn.data.multiname.ns_set]);
    default:
        return "";
    }
}

std::string AbcFile::fqn(Class& klass) {
    auto& mn    = cpool.multinames[klass.name];
    auto namesp = ns(mn);
    auto name   = mn.data.qname.name == 0 ? "*" : cpool.strings[mn.data.qname.name];
    return namesp.empty() ? name : namesp + "::" + name;
}
}