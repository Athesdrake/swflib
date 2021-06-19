#pragma once
#include "abc/common.h"

namespace swf::abc {
class Exception {
public:
    uint32_t from;
    uint32_t to;
    uint32_t target;
    uint32_t type;
    uint32_t var_name;

    Exception(AbcFile* abc);
    Exception() : Exception(nullptr) { }

    void read(StreamReader& stream);
    void write(StreamWriter& stream);

    std::string_view get_var_name();
    void rename_var_name(const char* name);
    void rename_var_name(std::string& name);

private:
    AbcFile* abc;
};
}