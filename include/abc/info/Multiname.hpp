#pragma once
#include "abc/common.hpp"

namespace swf::abc {
enum class MultinameKind : uint8_t {
    QName       = 0x07,
    QNameA      = 0x0D,
    RTQName     = 0x0F,
    RTQNameA    = 0x10,
    RTQNameL    = 0x11,
    RTQNameLA   = 0x12,
    Multiname   = 0x09,
    MultinameA  = 0x0E,
    MultinameL  = 0x1B,
    MultinameLA = 0x1C,
    Typename    = 0x1D
};

class Multiname {
public:
    MultinameKind kind;

    union MultinameData {
        struct QName {
            uint32_t ns;
            uint32_t name;
        } qname;

        struct RTQName {
            uint32_t name;
        } rtqname;

        struct Multiname {
            uint32_t name;
            uint32_t ns_set;
        } multiname;

        struct MultinameL {
            uint32_t ns_set;
        } multiname_l;

        struct TypeName {
            uint32_t qname;
        } type_name;
    } data;

    std::vector<uint32_t> types;

    Multiname();

    void read(StreamReader& stream);
    void write(StreamWriter& stream);

    uint32_t get_name_index();
    void set_name_index(uint32_t index);
};
}