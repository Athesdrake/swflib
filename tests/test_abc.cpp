#include "swflib.hpp"
#include "test.hpp"
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
        std::cout << "Version: " << (int)movie.version << "\nSymbols:\n";
        for (auto symbol : movie.symbol_class->symbols) {
            std::cout << '\t' << symbol.first << " - " << symbol.second << '\n';
        }

        arg = arg.substr(std::max(static_cast<int>(arg.find('/')), 0), arg.find('.'));
        for (auto it : movie.abcfiles) {
            auto abcfile = it.second->abcfile;

            abcfile->cpool.integers[1] = 666;
            abcfile->classes[1].rename("$\x45$");

            std::cout << "Abc File: " << it.first << std::endl;
            std::cout << "\tversion: " << abcfile->major_version << ":" << abcfile->minor_version << std::endl;
            std::cout << "\tscripts: " << abcfile->scripts.size() << std::endl;
            std::cout << "\tclasses: " << abcfile->classes.size() << std::endl;
            std::cout << "\tmethods: " << abcfile->methods.size() << std::endl;
        }

        swf::StreamWriter writer;
        movie.write(writer);
        writer.tofile("output-" + arg + "-abc.swf");

        stream = swf::StreamReader::fromfile("output-" + arg + "-abc.swf");

        for (auto it : movie.abcfiles) {
            auto abcfile = it.second->abcfile;

            assert_equal(abcfile->cpool.integers[1], 666);
            assert_equal(abcfile->classes[1].get_name(), "$\x45$");
        }
    }

    return 0;
}
