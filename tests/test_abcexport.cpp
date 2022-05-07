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

        arg = arg.substr(std::max(static_cast<int>(arg.find('/')), 0), arg.find('.'));
        for (auto it : movie.abcfiles) {
            auto abcfile = it.second->abcfile;
            swf::StreamWriter writer;
            abcfile->write(writer);
            writer.tofile(arg + '-' + it.first + ".abc");
        }
    }

    return 0;
}
