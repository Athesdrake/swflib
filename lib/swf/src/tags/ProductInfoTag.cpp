#include "swf/tags/ProductInfoTag.hpp"

namespace swf {
void ProductInfoTag::read(StreamReader&& stream) {
    productId     = stream.readU32();
    edition       = stream.readU32();
    version.major = stream.readU8();
    version.minor = stream.readU8();
    build         = stream.readU64();
    compileDate   = stream.readU64();
}
void ProductInfoTag::write(StreamWriter& stream) {
    stream.writeU32(productId);
    stream.writeU32(edition);
    stream.writeU8(version.major);
    stream.writeU8(version.minor);
    stream.writeU64(build);
    stream.writeU64(compileDate);
}
}