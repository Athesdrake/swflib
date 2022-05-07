#include "swf/tags/MetadataTag.hpp"

namespace swf {
void MetadataTag::read(StreamReader&& stream) { metadata = stream.readString(); }
void MetadataTag::write(StreamWriter& stream) { stream.writeString(metadata); }
}