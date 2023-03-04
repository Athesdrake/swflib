#pragma once
#include "abc/common.hpp"

namespace swf::abc {
struct MetadataItem {
    uint32_t key;
    uint32_t value;

    MetadataItem();
    MetadataItem(uint32_t value);
    MetadataItem(uint32_t key, uint32_t value);

    /* return true if the item is key-less */
    bool keyless() const;
};

class Metadata {
public:
    uint32_t name;
    std::vector<MetadataItem> items;

    Metadata(AbcFile* abc);
    Metadata() : Metadata(nullptr) { }

    void read(StreamReader& stream);
    void write(StreamWriter& stream);

    std::string_view get_name();
    void rename(const std::string& name);

private:
    AbcFile* abc;
};
}