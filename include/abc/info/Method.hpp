#pragma once
#include "Exception.hpp"
#include "Trait.hpp"
#include "abc/common.hpp"

namespace swf::abc {
struct Option {
    uint32_t value;
    uint8_t kind;
};

enum class MethodFlag : uint8_t {
    // Suggests to the runtime that an `arguments` object (as specified by the ActionScript 3.0 Language Reference) be
    // created. Must not be used together with NeedRest.
    NeedArguments = 0x01,
    // Must be set if this method uses the newactivation opcode.
    NeedActivation = 0x02,
    // This flag creates an ActionScript 3.0 rest arguments array.Must not be used with NeedArguments.
    NeedRest = 0x04,
    // Must be set if this method has optional parameters and the options field is present in this
    // method_info structure.
    HasOptional = 0x08,
    // Must be set if this method uses the dxns or dxnslate opcodes.
    SetDxns = 0x40,
    // Must be set when the param_names field is present in this method_info structure.
    HasParamNames = 0x80
};
constexpr uint8_t operator~(const MethodFlag lhs) { return ~static_cast<uint8_t>(lhs); }
constexpr uint8_t operator&(const uint8_t lhs, const MethodFlag rhs) { return lhs & static_cast<uint8_t>(rhs); }
constexpr uint8_t operator|(const uint8_t lhs, const MethodFlag rhs) { return lhs | static_cast<uint8_t>(rhs); }
constexpr uint8_t operator&(const MethodFlag lhs, const MethodFlag rhs) { return static_cast<uint8_t>(lhs) & rhs; }
constexpr uint8_t operator|(const MethodFlag lhs, const MethodFlag rhs) { return static_cast<uint8_t>(lhs) | rhs; }
constexpr uint8_t operator&=(uint8_t& lhs, const MethodFlag rhs) { return lhs &= static_cast<uint8_t>(rhs); }
constexpr uint8_t operator|=(uint8_t& lhs, const MethodFlag rhs) { return lhs |= static_cast<uint8_t>(rhs); }

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