#pragma once
#include "Exception.h"
#include "Trait.h"
#include "abc/common.h"

#define NEED_ARGUMENTS                                                                                                 \
    0x01 // Suggests to the run - time that an �arguments� object(as specified by the ActionScript 3.0 Language
         // Reference) be created.Must not be used together with NEED_REST.
#define NEED_ACTIVATION 0x02 // Must be set if this method uses the newactivation opcode.
#define NEED_REST                                                                                                      \
    0x04 // This flag creates an ActionScript 3.0 rest arguments array.Must not be used with NEED_ARGUMENTS.
#define HAS_OPTIONAL                                                                                                   \
    0x08 // Must be set if this method has optional parameters and the options field is present in this method_info
         // structure.
#define SET_DXNS 0x40 // Must be set if this method uses the dxns or dxnslate opcodes.
#define HAS_PARAM_NAMES 0x80 // Must be set when the param_names field is present in this method_info structure.

namespace swf::abc {
struct Option {
    uint32_t value;
    uint8_t kind;
};

class Method {
public:
    uint32_t return_type;
    uint32_t name;
    uint8_t flags;
    std::vector<Option> optional;
    std::vector<std::string> param_names;
    std::vector<uint32_t> params;

    uint32_t max_stack;
    uint32_t local_count;
    uint32_t init_scope_depth;
    uint32_t max_scope_depth;
    std::vector<uint8_t> code;
    std::vector<Exception> exceptions;
    std::vector<Trait> traits;

    Method(AbcFile* abc);
    Method() : Method(nullptr) { }

    void read(StreamReader& stream);
    void read_body(StreamReader& stream);
    void write(StreamWriter& stream);
    void write_body(StreamWriter& stream);

    void checkFlags();

    bool has_body() const;
    bool has_optional() const;
    bool has_param_names() const;
    bool need_rest() const;

    std::string_view get_name();
    void rename(std::string name);
    std::string_view get_param_name(size_t index);

private:
    AbcFile* abc;
};
}