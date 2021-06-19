#include "swf/tags/DefineBinaryDataTag.h"

namespace swf {
StreamReader* DefineBinaryDataTag::getData() { return binary.get(); }

void DefineBinaryDataTag::read(StreamReader&& stream) {
    charId = stream.readU16();
    stream.readU32();

    uint32_t length = (uint32_t)(stream.endpos() - stream.raw());

    binary = std::make_unique<StreamReader>(stream.raw(), stream.raw() + length, false);
}
void DefineBinaryDataTag::write(StreamWriter& stream) {
    binary->reset();
    stream.writeU16(charId);
    stream.writeU32(0);
    stream.write(*binary);
}
}