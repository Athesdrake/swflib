#include "abc/parser/Parser.hpp"
#include "parse.hpp"

namespace swf::abc::parser {
Parser::Parser(Method& method) : method(method) {
    auto stream = StreamReader(method.code);

    std::unordered_multimap<uint32_t, std::pair<Instruction*, size_t>> targets;
    std::shared_ptr<Instruction> current;
    while (stream) {
        auto ins = parse(stream, targets);
        if (begin == nullptr)
            begin = current = ins;
        else {
            current->next = ins;
            ins->prev     = current;
            current       = ins;
        }
    }
    end = current;

    if (!targets.empty()) {
        std::weak_ptr<Instruction> ins = current;
        while (current = ins.lock()) {
            auto its = targets.equal_range(current->addr);
            for (auto it = its.first; it != its.second; ++it)
                it->second.first->targets[it->second.second] = current;

            ins = current->prev;
        }
    }
}
}