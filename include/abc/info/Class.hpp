#pragma once
#include "Trait.hpp"
#include "abc/common.hpp"

#define CONSTANT_ClassSealed                                                                                           \
    0x01 // The class is sealed : properties can not be dynamically added to instances of the class.
#define CONSTANT_ClassFinal 0x02 // The class is final : it cannot be a base class for any other class.
#define CONSTANT_ClassInterface 0x04 // The class is an interface.
#define CONSTANT_ClassProtectedNs                                                                                      \
    0x08 // The class uses its protected namespace and the protectedNs field is present in the interface_info structure.

namespace swf::abc {
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

    bool isSealed() { return flags & CONSTANT_ClassSealed; }
    bool isFinal() { return flags & CONSTANT_ClassFinal; }
    bool isInterface() { return flags & CONSTANT_ClassInterface; }
    bool isProtected() { return flags & CONSTANT_ClassProtectedNs; }

    void setSealed(bool active) { flags = (flags & ~CONSTANT_ClassSealed) | (active << 0); }
    void setFinal(bool active) { flags = (flags & ~CONSTANT_ClassFinal) | (active << 1); }
    void setInterface(bool active) { flags = (flags & ~CONSTANT_ClassInterface) | (active << 2); }
    void setProtected(bool active) { flags = (flags & ~CONSTANT_ClassProtectedNs) | (active << 3); }

    std::string get_name();
    std::string get_super_name();
    void rename(const std::string& name);
    void rename_super(const std::string& name);

private:
    AbcFile* abc;
};
}