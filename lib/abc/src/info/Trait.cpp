#include "abc/info/Trait.h"
#include "abc/AbcFile.h"
#include <stdexcept>

namespace swf::abc {
Trait::Trait(AbcFile* abc) : abc(abc) {
    name  = 0;
    attr  = 0;
    index = 0;
    kind  = TraitKind::Slot;

    slot_id   = 0;
    slot.kind = 0;
    slot.type = 0;
}

void Trait::read(StreamReader& stream) {
    uint8_t kkind;

    name  = stream.readU30();
    kkind = stream.readU8();
    kind  = TraitKind(kkind & 0x0f);
    attr  = kkind >> 4;

    switch (kind) {
    case TraitKind::Slot:
    case TraitKind::Const:
        slot_id   = stream.readU30();
        slot.type = stream.readU30();

        if ((index = stream.readU30()))
            slot.kind = stream.readU8();

        break;
    case TraitKind::Method:
    case TraitKind::Getter:
    case TraitKind::Setter:
    case TraitKind::Class:
    case TraitKind::Function:
        slot_id = stream.readU30();
        index   = stream.readU30();
        break;
    default:
        throw std::runtime_error("Unknown Trait kind.");
        break;
    }

    if (attr & Metadata) { // Metadata is useless so we ignore it
        auto count = stream.readU30();
        for (uint32_t i = 0; i < count; i++)
            stream.skip30();
    }
}

void Trait::write(StreamWriter& stream) {
    stream.writeU30(name);
    stream.writeU8((attr << 4) | uint8_t(kind));

    stream.writeU30(slot_id);
    switch (kind) {
    case TraitKind::Slot:
    case TraitKind::Const:
        stream.writeU30(slot.type);
        stream.writeU30(index);
        if (index)
            stream.writeU8(slot.kind);
        break;
    case TraitKind::Method:
    case TraitKind::Getter:
    case TraitKind::Setter:
    case TraitKind::Class:
    case TraitKind::Function:
        stream.writeU30(index);
        break;
    default:
        throw std::runtime_error("Unknown Trait kind.");
        break;
    }

    if (attr & Metadata)
        stream.writeU30(0);
}

std::string Trait::toString() {
    std::string value = "<{" + std::to_string(slot_id) + "} ";

    switch (kind) {
    case TraitKind::Slot:
        value += "Slot:";
        break;
    case TraitKind::Const:
        value += "Const:";
        break;
    case TraitKind::Method:
        value += "Method";
        break;
    case TraitKind::Getter:
        value += "Getter";
        break;
    case TraitKind::Setter:
        value += "Setter";
        break;
    case TraitKind::Class:
        value += "Class";
        break;
    case TraitKind::Function:
        value += "Function";
        break;
    default:
        throw std::runtime_error("Unknown Trait kind.");
        break;
    }

    if (kind == TraitKind::Slot || kind == TraitKind::Const) {
        if (slot.type == 0)
            value += "*";
        else
            value += abc->cpool.strings[abc->cpool.multinames[slot.type].data.qname.name];
    }

    if (index != 0)
        value += " index=" + std::to_string(index);

    return value + ">";
}

std::string_view Trait::get_name() {
    if (name == 0)
        return "";

    auto index = abc->cpool.multinames[name].data.qname.name;
    return abc->cpool.strings[index];
}

void Trait::rename(const std::string& name) {
    auto index                = abc->cpool.multinames[this->name].data.qname.name;
    abc->cpool.strings[index] = name;
}
}