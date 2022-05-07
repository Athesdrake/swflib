#pragma once
#include "Trait.hpp"
#include "abc/common.hpp"

namespace swf::abc {
class Script {
public:
    uint32_t init;
    std::vector<Trait> traits;

    Script(AbcFile* abc);
    Script() : Script(nullptr) { }

    void read(StreamReader& stream);
    void write(StreamWriter& stream);

private:
    AbcFile* abc;
};
}