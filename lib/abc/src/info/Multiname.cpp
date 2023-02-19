#include "abc/info/Multiname.hpp"
#include "abc/AbcFile.hpp"
#include <stdexcept>

namespace swf::abc {
Multiname::Multiname() {
    kind  = MultinameKind::QName;
    types = {};

    data.qname.ns   = 0;
    data.qname.name = 0;
}

void Multiname::read(StreamReader& stream) {
    kind = MultinameKind(stream.readU8());

    switch (kind) {
    case MultinameKind::QName:
    case MultinameKind::QNameA:
        data.qname.ns   = stream.readU30();
        data.qname.name = stream.readU30();
        break;
    case MultinameKind::RTQName:
    case MultinameKind::RTQNameA:
        data.rtqname.name = stream.readU30();
        break;
    case MultinameKind::RTQNameL:
    case MultinameKind::RTQNameLA:
        // This kind has no associated data.
        break;
    case MultinameKind::Multiname:
    case MultinameKind::MultinameA:
        data.multiname.name   = stream.readU30();
        data.multiname.ns_set = stream.readU30();
        break;
    case MultinameKind::MultinameL:
    case MultinameKind::MultinameLA:
        data.multiname_l.ns_set = stream.readU30();
        break;
    case MultinameKind::Typename: {
        data.type_name.qname = stream.readU30();
        types.resize(stream.readU30());
        for (auto& type : types)
            type = stream.readU30();

        break;
    }
    default:
        throw std::runtime_error("Unknown multiname kind.");
        break;
    }
}

void Multiname::write(StreamWriter& stream) {
    stream.writeU8(static_cast<uint8_t>(kind));

    switch (kind) {
    case MultinameKind::QName:
    case MultinameKind::QNameA:
        stream.writeU30(data.qname.ns);
        stream.writeU30(data.qname.name);
        break;
    case MultinameKind::RTQName:
    case MultinameKind::RTQNameA:
        stream.writeU30(data.rtqname.name);
        break;
    case MultinameKind::RTQNameL:
    case MultinameKind::RTQNameLA:
        // This kind has no associated data.
        break;
    case MultinameKind::Multiname:
    case MultinameKind::MultinameA:
        stream.writeU30(data.multiname.name);
        stream.writeU30(data.multiname.ns_set);
        break;
    case MultinameKind::MultinameL:
    case MultinameKind::MultinameLA:
        stream.writeU30(data.multiname_l.ns_set);
        break;
    case MultinameKind::Typename:
        stream.writeU30(data.type_name.qname);
        stream.writeU30((uint32_t)types.size());
        for (auto type : types)
            stream.writeU30(type);
        break;
    default:
        throw std::runtime_error("Unknown multiname kind.");
        break;
    }
}

uint32_t Multiname::get_name_index() {
    switch (kind) {
    case MultinameKind::QName:
    case MultinameKind::QNameA:
        return data.qname.name;
    case MultinameKind::RTQName:
    case MultinameKind::RTQNameA:
        return data.rtqname.name;
    case MultinameKind::RTQNameL:
    case MultinameKind::RTQNameLA:
        return 0;
    case MultinameKind::Multiname:
    case MultinameKind::MultinameA:
        return data.multiname.name;
    case MultinameKind::MultinameL:
    case MultinameKind::MultinameLA:
    case MultinameKind::Typename:
    default:
        throw std::runtime_error("Unknown multiname kind.");
        break;
    }
}

void Multiname::set_name_index(uint32_t index) {
    switch (kind) {
    case MultinameKind::QName:
    case MultinameKind::QNameA:
        data.qname.name = index;
        break;
    case MultinameKind::RTQName:
    case MultinameKind::RTQNameA:
        data.rtqname.name = index;
        break;
    case MultinameKind::Multiname:
    case MultinameKind::MultinameA:
        data.multiname.name = index;
        break;
    case MultinameKind::RTQNameL:
    case MultinameKind::RTQNameLA:
    case MultinameKind::MultinameL:
    case MultinameKind::MultinameLA:
    case MultinameKind::Typename:
    default:
        throw std::runtime_error("Unknown multiname kind.");
        break;
    }
}
}