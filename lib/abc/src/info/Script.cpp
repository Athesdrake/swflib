#include "abc/info/Script.hpp"
#include "abc/AbcFile.hpp"

namespace swf::abc {
Script::Script(AbcFile* abc) : abc(abc) {
    init   = 0;
    traits = std::vector<Trait>();
}

void Script::read(StreamReader& stream) {
    init = stream.readU30();
    traits.resize(stream.readU30());
    for (auto& trait : traits) {
        trait = Trait(abc);
        trait.read(stream);
    }
}

void Script::write(StreamWriter& stream) {
    stream.writeU30(init);
    stream.writeU30((uint32_t)traits.size());
    for (auto trait : traits)
        trait.write(stream);
}
}