#pragma once
#include "abc/parser/Instruction.h"

namespace swf::abc::parser {
/*Utility class that parse the given method's code.
You can get the first instruction with `begin` and the last with `end`.

Example
-------

.. code-block:: cpp
    Parser parser(method);
    auto ins = parser.begin;
    while (ins) {
        std::cout << opnames[static_cast<uint8_t>(ins.opcode)] << std::endl;
        ins = ins.next;
    }
*/
class Parser {
public:
    Method method;

    std::shared_ptr<Instruction> begin; // First instruction in the method's code
    std::shared_ptr<Instruction> end; // Last instruction
    Parser(Method& method);
};
}