#include "abc/info/Metadata.hpp"
#include "abc/AbcFile.hpp"

namespace swf::abc {
MetadataItem::MetadataItem() : MetadataItem(0) { }
MetadataItem::MetadataItem(uint32_t value) : MetadataItem(0, value) { }
MetadataItem::MetadataItem(uint32_t key, uint32_t value) : key(key), value(value) { }
bool MetadataItem::keyless() const { return key == 0; }

Metadata::Metadata(AbcFile* abc) : abc(abc) { }

void Metadata::read(StreamReader& stream) {
    this->name = stream.readU30();
    auto count = stream.readU30();
    items.resize(count);

    for (uint32_t i = 0; i < count; i++)
        items[i].key = stream.readU30();

    for (uint32_t i = 0; i < count; i++)
        items[i].value = stream.readU30();
}
void Metadata::write(StreamWriter& stream) {
    stream.writeU30(name);
    stream.writeU30(items.size());

    for (const auto& item : items)
        stream.writeU30(item.key);

    for (const auto& item : items)
        stream.writeU30(item.value);
}

std::string_view Metadata::get_name() { return abc->cpool.strings[name]; }
void Metadata::rename(const std::string& name) { abc->cpool.strings[this->name] = name; }
}