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

enum class TraitAttr : uint8_t {
    // Used with Trait_Method, Trait_Getter and Trait_Setter.
    // It marks a method that cannot be overridden by a sub-class
    Final = 0x01,
    // Used with Trait_Method, Trait_Getter and Trait_Setter
    // It marks a method that has been overridden in this class
    Override = 0x02,
    // Used to signal that the fields metadata_count and metadata follow the data field in the traits_info entry
    Metadata = 0x04,
};
constexpr uint8_t operator~(const TraitAttr lhs) { return ~static_cast<uint8_t>(lhs); }
constexpr uint8_t operator&(const uint8_t lhs, const TraitAttr rhs) { return lhs & static_cast<uint8_t>(rhs); }
constexpr uint8_t operator|(const uint8_t lhs, const TraitAttr rhs) { return lhs | static_cast<uint8_t>(rhs); }
constexpr uint8_t operator&(const TraitAttr lhs, const TraitAttr rhs) { return static_cast<uint8_t>(lhs) & rhs; }
constexpr uint8_t operator|(const TraitAttr lhs, const TraitAttr rhs) { return static_cast<uint8_t>(lhs) | rhs; }

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
    std::vector<uint32_t> metadatas;

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