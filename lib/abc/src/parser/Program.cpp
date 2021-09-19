#include "abc/parser/Program.h"

namespace swf::abc::parser {
Program::Program(std::shared_ptr<AbcFile> abc, Method& method) : abc(abc), method(method) {
    stream = StreamReader(method.code);
}

Program::operator bool() const { return has_next(); }
bool Program::has_next() const { return stream; }
}