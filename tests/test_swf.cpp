#include "swflib.h"
#include "test.h"
#include <algorithm>
#include <cmath>

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
        std::cout << "Version: " << (int)movie.version << "\nSymbols:\n";
        for (auto symbol : movie.symbol_class->symbols) {
            std::cout << '\t' << symbol.first << " - " << symbol.second << '\n';
        }

        arg = arg.substr(std::max(static_cast<int>(arg.find('/')), 0), arg.find('.'));
        for (auto c : { swf::Compression::None, swf::Compression::Zlib, swf::Compression::Lzma }) {
            swf::StreamWriter writer;
            std::string compression;

            switch (c) {
            case swf::Compression::None:
                compression = "raw";
                break;
            case swf::Compression::Zlib:
                compression = "zlib";
                break;
            case swf::Compression::Lzma:
                compression = "lzma";
                break;
            }

            movie.signature[0] = char(c);
            movie.write(writer);
            auto filename = "output-" + arg + '-' + compression + ".swf";
            writer.tofile(filename);

            swf::Swf().read(*swf::StreamReader::fromfile(filename));
        }
    }
    return 0;
}
