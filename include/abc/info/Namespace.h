#pragma once
#include "abc/common.h"

namespace swf::abc {
enum class NamespaceKind : uint8_t {
    STAR            = 0x0,
    Namespace       = 0x08,
    Package         = 0x16,
    PackageInternal = 0x17,
    Protected       = 0x18,
    Explicit        = 0x19,
    StaticProtected = 0x1A,
    Private         = 0x05
};

class Namespace {
public:
    NamespaceKind kind;
    uint32_t name;

    Namespace();

    void read(StreamReader& stream);
    void write(StreamWriter& stream);
};
}