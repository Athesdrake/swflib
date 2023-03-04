#pragma once
#include "Trait.hpp"
#include "abc/common.hpp"

namespace swf::abc {
enum class ClassFlag : uint8_t {
    // The class is sealed : properties can not be dynamically added to instances of the class.
    Sealed = 0x01,
    // The class is final : it cannot be a base class for any other class.
    Final = 0x02,
    // The class is an interface.
    Interface = 0x04,
    // The class uses its protected namespace and the protectedNs field is present in the interface_info structure.
    ProtectedNamespace = 0x08,
};
constexpr uint8_t operator~(const ClassFlag lhs) { return ~static_cast<uint8_t>(lhs); }
constexpr uint8_t operator&(const uint8_t lhs, const ClassFlag rhs) { return lhs & static_cast<uint8_t>(rhs); }
constexpr uint8_t operator|(const uint8_t lhs, const ClassFlag rhs) { return lhs | static_cast<uint8_t>(rhs); }
constexpr uint8_t operator&(const ClassFlag lhs, const ClassFlag rhs) { return static_cast<uint8_t>(lhs) & rhs; }
constexpr uint8_t operator|(const ClassFlag lhs, const ClassFlag rhs) { return static_cast<uint8_t>(lhs) | rhs; }

class Class {
public:
    uint32_t name;
    uint32_t super_name;
    uint8_t flags;
    uint32_t protected_ns;
    uint32_t iinit;
    uint32_t cinit;
    std::vector<uint32_t> interfaces;
    std::vector<Trait> itraits;
    std::vector<Trait> ctraits;

    Class(AbcFile* abc);
    Class() : Class(nullptr) { }

    void read(StreamReader& stream);
    void read_instance(StreamReader& stream);
    void write(StreamWriter& stream);
    void write_instance(StreamWriter& stream);

    bool isSealed() { return flags & ClassFlag::Sealed; }
    bool isFinal() { return flags & ClassFlag::Final; }
    bool isInterface() { return flags & ClassFlag::Interface; }
    bool isProtected() { return flags & ClassFlag::ProtectedNamespace; }

    void setSealed(bool active) { flags = (flags & ~ClassFlag::Sealed) | (active << 0); }
    void setFinal(bool active) { flags = (flags & ~ClassFlag::Final) | (active << 1); }
    void setInterface(bool active) { flags = (flags & ~ClassFlag::Interface) | (active << 2); }
    void setProtected(bool active) { flags = (flags & ~ClassFlag::ProtectedNamespace) | (active << 3); }

    std::string get_name();
    std::string get_super_name();
    void rename(const std::string& name);
    void rename_super(const std::string& name);

private:
    AbcFile* abc;
};
}