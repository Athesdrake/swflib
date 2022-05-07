#include "swf/tags/SetBackgroundColorTag.hpp"

namespace swf {
void SetBackgroundColorTag::read(StreamReader&& stream) { color.read(stream); }
void SetBackgroundColorTag::write(StreamWriter& stream) { color.write(stream); }
}