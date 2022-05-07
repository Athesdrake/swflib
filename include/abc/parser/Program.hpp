#pragma once
#include "abc/parser/Instruction.hpp"

namespace swf::abc::parser {
class Program {
public:
    Method method;

    Program(std::shared_ptr<AbcFile> abc, Method& method);

    operator bool() const;
    bool has_next() const;
    uint32_t peek() const;

    uint32_t jump(uint32_t pos);

private:
    std::shared_ptr<AbcFile> abc;
    StreamReader stream;
};
}