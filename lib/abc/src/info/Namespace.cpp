#include "abc/info/Namespace.h"
#include "abc/AbcFile.h"

namespace swf::abc {
Namespace::Namespace()
{
    kind = NamespaceKind::STAR;
    name = 0;
}

void Namespace::read(StreamReader& stream)
{
    kind = NamespaceKind(stream.readU30());
    name = stream.readU30();
}

void Namespace::write(StreamWriter& stream)
{
    stream.writeU30((uint8_t)kind);
    stream.writeU30(name);
}
}