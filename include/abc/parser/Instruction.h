#pragma once
#include "abc/AbcFile.h"
#include "abc/parser/opcodes.h"

namespace swf::abc::parser {
class Instruction {
public:
    OP opcode;
    uint32_t addr;

    std::shared_ptr<Instruction> next;
    std::weak_ptr<Instruction> prev;
    std::vector<std::weak_ptr<Instruction>> targets;
    std::vector<uint32_t> args;

    Instruction(OP opcode, uint32_t addr);
    Instruction(uint8_t opcode, uint32_t addr) : Instruction(OP(opcode), addr) { }

    // Returns true if the instruction is a jump instruction
    bool isJump() const;
};
}