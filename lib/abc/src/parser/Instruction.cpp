#include "abc/parser/Instruction.hpp"

namespace swf::abc::parser {
Instruction::Instruction(OP opcode, uint32_t addr) : opcode(opcode), addr(addr) { }

bool Instruction::isJump() const { return opcode >= OP::ifnlt && opcode <= OP::lookupswitch; }
}