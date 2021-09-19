#include "abc/parser/Parser.h"
#include "swflib.h"
#include "test.h"
#include <algorithm>

int main(int argc, char* argv[]) {
    std::vector<std::string> args(argv + 1, argv + argc);

    if (args.empty()) {
        std::string executable = argv[0];
        std::replace(executable.begin(), executable.end(), '\\', '/');
        auto pos = executable.rfind('/');

        if (pos != -1)
            executable = executable.substr(pos + 1);

        std::cout << "Usage: " << executable << " file1.swf file2.swf ..." << std::endl;
        return 1;
    }

    for (auto arg : args) {
        std::cout << "File: " << arg << std::endl;
        swf::Swf movie;

        auto stream = swf::StreamReader::fromfile(arg);
        movie.read(*stream);

        for (auto it : movie.abcfiles) {
            auto abcfile = it.second->abcfile;

            for (auto& method : abcfile->methods) {
                if (method.has_body()) {
                    std::cout << "code size: " << method.code.size() << std::endl;
                    // auto program = swf::abc::Program(abcfile, method);
                    swf::abc::parser::Parser parser(method);

                    auto ins = parser.begin;
                    while (ins) {
                        std::cout << swf::abc::parser::opnames[static_cast<uint8_t>(ins->opcode)] << std::endl;
                        for (auto i = 0; i < ins->targets.size(); ++i) {
                            auto target = ins->targets[i];
                            // check if target is nullptr (but don't throw any error if the target is out of bounds)
                            using wt = std::weak_ptr<swf::abc::parser::Instruction>;
                            assert(target.owner_before(wt {}) || wt {}.owner_before(target)
                                || ins->args[i] >= method.code.size());
                        }
                        ins = ins->next;
                    }
                }
            }
        }
    }

    return 0;
}
