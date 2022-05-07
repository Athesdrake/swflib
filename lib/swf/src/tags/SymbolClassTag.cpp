#include "swf/tags/SymbolClassTag.hpp"
#include <unordered_map>

namespace swf {
uint16_t SymbolClassTag::getCharId(std::string name) {
    return std::find_if(std::begin(symbols), std::end(symbols), [&](const std::pair<uint16_t, std::string>& pair) {
        return pair.second == name;
    })->first;
}

void SymbolClassTag::read(StreamReader&& stream) {
    uint16_t count = stream.readU16();
    for (uint16_t i = 0; i < count; ++i) {
        uint16_t id = stream.readU16();
        symbols[id] = stream.readCharArray();
    }
}
void SymbolClassTag::write(StreamWriter& stream) {
    stream.writeU16((uint16_t)symbols.size());
    for (auto it : symbols) {
        stream.writeU16(it.first);
        stream.writeCharArray(it.second);
    }
}
}