#include "abc/parser/Instruction.hpp"
#include <unordered_map>

namespace swf::abc::parser {
std::shared_ptr<Instruction> parse(
    StreamReader& stream, std::unordered_multimap<uint32_t, std::pair<Instruction*, size_t>>& jump_targets);
}