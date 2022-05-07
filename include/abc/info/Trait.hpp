#pragma once
#include "abc/common.hpp"

namespace swf::abc {
enum class TraitKind : uint8_t {
    Slot     = 0,
    Method   = 1,
    Getter   = 2,
    Setter   = 3,
    Class    = 4,
    Function = 5,
    Const    = 6,
};

enum TraitAttributes {
    Final = 0x01, // Is used with Trait_Method, Trait_Getter and Trait_Setter.It marks a method that cannot be
                  // overridden by a sub - class
    Override = 0x02, // Is used with Trait_Method, Trait_Getter and Trait_Setter.It marks a method that has been
                     // overridden in this class
    Metadata = 0x04, // Is used to signal that the fields metadata_count and metadata follow the data field in the
                     // traits_info entry
};

class Trait {
public:
    uint32_t name;
    TraitKind kind;
    uint8_t attr;

    uint32_t slot_id;
    uint32_t index;

    struct Slot {
        uint32_t type;
        uint8_t kind;
    } slot;
    // std::vector<uint32_t> metadatas;

    Trait(AbcFile* abc);
    Trait() : Trait(nullptr) { }

    void read(StreamReader& stream);
    void write(StreamWriter& stream);

    std::string toString();

    std::string_view get_name();
    void rename(const std::string& name);

private:
    AbcFile* abc;
};
}