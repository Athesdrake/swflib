#include "swf/tags/DoABCTag.h"
#include "abc/AbcFile.h"

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