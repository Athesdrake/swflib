#include "swf/tags/ScriptLimitsTag.h"

namespace swf {
void ScriptLimitsTag::read(StreamReader&& stream) {
    max_recursion_depth    = stream.readU16();
    script_timeout_seconds = stream.readU16();
}
void ScriptLimitsTag::write(StreamWriter& stream) {
    stream.writeU16(max_recursion_depth);
    stream.writeU16(script_timeout_seconds);
}
}