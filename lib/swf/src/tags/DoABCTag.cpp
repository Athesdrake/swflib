#include "swf/tags/DoABCTag.hpp"
#include "abc/AbcFile.hpp"

namespace swf {
void DoABCTag::read(StreamReader&& stream) {
    is_lazy = stream.readU32() & 1;
    name    = stream.readCharArray();
    abcfile = std::make_shared<abc::AbcFile>(stream);
}
void DoABCTag::write(StreamWriter& stream) {
    stream.writeU32((uint32_t)is_lazy);
    stream.writeCharArray(name);
    abcfile->write(stream);
}
}