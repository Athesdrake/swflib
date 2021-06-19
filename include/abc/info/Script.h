#pragma once
#include "Trait.h"
#include "abc/common.h"

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