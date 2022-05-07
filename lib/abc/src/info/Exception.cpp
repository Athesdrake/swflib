#include "abc/info/Exception.hpp"
#include "abc/AbcFile.hpp"

namespace swf::abc {
Exception::Exception(AbcFile* abc) : abc(abc) {
    from     = 0;
    to       = 0;
    target   = 0;
    type     = 0;
    var_name = 0;
}

void Exception::read(StreamReader& stream) {
    from     = stream.readU30();
    to       = stream.readU30();
    target   = stream.readU30();
    type     = stream.readU30();
    var_name = stream.readU30();
}

void Exception::write(StreamWriter& stream) {
    stream.writeU30(from);
    stream.writeU30(to);
    stream.writeU30(target);
    stream.writeU30(type);
    stream.writeU30(var_name);
}

std::string_view Exception::get_var_name() {
    if (var_name == 0)
        return "";

    return abc->cpool.strings[abc->cpool.multinames[var_name].data.qname.name];
}

void Exception::rename_var_name(const char* name) {
    if (var_name == 0)
        return;

    abc->cpool.strings[abc->cpool.multinames[var_name].data.qname.name] = name;
}
void Exception::rename_var_name(std::string& name) { rename_var_name(name.c_str()); }
}