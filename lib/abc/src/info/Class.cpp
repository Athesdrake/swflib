#include "abc/info/Class.hpp"
#include "abc/AbcFile.hpp"

namespace swf::abc {
Class::Class(AbcFile* abc) : abc(abc) {
    name  = 0;
    flags = 0;
    iinit = 0;
    cinit = 0;

    super_name   = 0;
    protected_ns = 0;

    interfaces = std::vector<uint32_t>();

    itraits = std::vector<Trait>();
    ctraits = std::vector<Trait>();
}

void Class::read(StreamReader& stream) {
    cinit = stream.readU30();
    ctraits.resize(stream.readU30());
    for (auto& trait : ctraits) {
        trait = Trait(abc);
        trait.read(stream);
    }
}

void Class::read_instance(StreamReader& stream) {
    name       = stream.readU30();
    super_name = stream.readU30();
    flags      = stream.readU8();

    if (isProtected())
        protected_ns = stream.readU30();

    interfaces.resize(stream.readU30());
    for (auto& i : interfaces)
        i = stream.readU30();

    iinit = stream.readU30();
    itraits.resize(stream.readU30());
    for (auto& trait : itraits) {
        trait = Trait(abc);
        trait.read(stream);
    }
}

void Class::write(StreamWriter& stream) {
    stream.writeU30(cinit);
    stream.writeU30((uint32_t)ctraits.size());
    for (auto trait : ctraits)
        trait.write(stream);
}

void Class::write_instance(StreamWriter& stream) {
    stream.writeU30(name);
    stream.writeU30(super_name);
    stream.writeU8(flags);

    if (isProtected())
        stream.writeU30(protected_ns);

    stream.writeU30((uint32_t)interfaces.size());
    for (auto i : interfaces)
        stream.writeU30(i);

    stream.writeU30(iinit);
    stream.writeU30((uint32_t)itraits.size());
    for (auto trait : itraits)
        trait.write(stream);
}

std::string Class::get_name() {
    auto index = abc->cpool.multinames[name].data.qname.name;
    return abc->cpool.strings[index];
}

std::string Class::get_super_name() {
    if (super_name == 0)
        return "";

    auto index = abc->cpool.multinames[super_name].data.qname.name;
    return abc->cpool.strings[index];
}

void Class::rename(const std::string& name) {
    auto index                = abc->cpool.multinames[this->name].data.qname.name;
    abc->cpool.strings[index] = name;
}

void Class::rename_super(const std::string& name) {
    if (super_name == 0)
        return;

    auto index                = abc->cpool.multinames[super_name].data.qname.name;
    abc->cpool.strings[index] = name;
}
}