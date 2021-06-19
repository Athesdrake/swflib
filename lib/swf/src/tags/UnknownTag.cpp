#include "swf/tags/UnknownTag.h"

namespace swf {
void UnknownTag::read(StreamReader&& stream) { data = std::move(stream); }
void UnknownTag::write(StreamWriter& stream) {
    data.reset();
    stream.write(data);
}
}