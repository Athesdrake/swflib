#include "abc/info/Method.h"
#include "abc/AbcFile.h"

namespace swf::abc {
Method::Method(AbcFile* abc) : abc(abc) {
    name  = 0;
    flags = 0;

    return_type = 0;
    optional    = std::vector<Option>();
    param_names = std::vector<std::string>();
    params      = std::vector<uint32_t>();

    max_stack   = 0;
    local_count = 0;

    init_scope_depth = 0;
    max_scope_depth  = 0;

    code       = {};
    exceptions = std::vector<Exception>();
    traits     = std::vector<Trait>();
}

void Method::read(StreamReader& stream) {
    params.resize(stream.readU30());
    return_type = stream.readU30();

    for (auto& param : params)
        param = stream.readU30();

    name  = stream.readU30();
    flags = stream.readU8();

    if (has_optional()) {
        optional.resize(stream.readU30());
        for (auto& option : optional) {
            option.value = stream.readU30();
            option.kind  = stream.readU30();
        }
    }
    if (has_param_names()) {
        param_names.resize(stream.readU30());
        for (auto& param : param_names)
            param = stream.readString();
    }
}

void Method::read_body(StreamReader& stream) {
    max_stack        = stream.readU30();
    local_count      = stream.readU30();
    init_scope_depth = stream.readU30();
    max_scope_depth  = stream.readU30();
    auto length      = stream.readU30();
    stream.copy(length, code);

    exceptions.resize(stream.readU30());
    for (auto& e : exceptions) {
        e = Exception(abc);
        e.read(stream);
    }

    traits.resize(stream.readU30());
    for (auto& trait : traits) {
        trait = Trait(abc);
        trait.read(stream);
    }
}

void Method::write(StreamWriter& stream) {
    stream.writeU30((uint32_t)params.size());
    stream.writeU30(return_type);

    for (auto& param : params)
        stream.writeU30(param);

    checkFlags();
    stream.writeU30(name);
    stream.writeU8(flags);

    if (has_optional()) {
        stream.writeU30((uint32_t)optional.size());
        for (auto& option : optional) {
            stream.writeU30(option.value);
            stream.writeU30(option.kind);
        }
    }
    if (has_param_names()) {
        stream.writeU30((uint32_t)param_names.size());
        for (auto& param : param_names)
            stream.writeString(param);
    }
}

void Method::write_body(StreamWriter& stream) {
    stream.writeU30(max_stack);
    stream.writeU30(local_count);
    stream.writeU30(init_scope_depth);
    stream.writeU30(max_scope_depth);
    stream.writeU30((uint32_t)code.size());
    stream.extend((uint32_t)code.size(), (uint8_t*)code.data());

    stream.writeU30((uint32_t)exceptions.size());
    for (auto& e : exceptions)
        e.write(stream);

    stream.writeU30((uint32_t)traits.size());
    for (auto& trait : traits)
        trait.write(stream);
}

void Method::checkFlags() {
    if (optional.size())
        flags |= HAS_OPTIONAL;

    if (param_names.size())
        flags |= HAS_PARAM_NAMES;
}

bool Method::has_optional() const { return flags & HAS_OPTIONAL; }

bool Method::has_param_names() const { return flags & HAS_PARAM_NAMES; }

bool Method::need_rest() const { return flags & NEED_REST; }

bool Method::has_body() const { return code.size(); }

std::string_view Method::get_name() {
    if (this->name == 0)
        return "";

    return abc->cpool.strings[this->name];
}

void Method::rename(std::string name) {
    if (this->name == 0)
        return;

    abc->cpool.strings[this->name] = name;
}

std::string_view Method::get_param_name(size_t index) {
    auto name = abc->cpool.multinames[params[index]].data.qname.name;
    if (name == 0)
        return "*";

    return abc->cpool.strings[name];
}
}