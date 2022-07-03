#include "parse.hpp"
#include <stdexcept>

namespace swf::abc::parser {
std::shared_ptr<Instruction> parse(
    StreamReader& stream, std::unordered_multimap<uint32_t, std::pair<Instruction*, size_t>>& jump_targets) {
    uint32_t addr = static_cast<uint32_t>(stream.pos());
    OP opcode     = OP(stream.readU8());
    auto ins      = std::make_shared<Instruction>(opcode, addr);
    int32_t target;

    switch (opcode) {
    case OP::bkpt:
    case OP::nop:
    case OP::OP_throw:
    case OP::dxnslate:
    case OP::label:
    case OP::lf32x4:
    case OP::sf32x4:
    case OP::pushwith:
    case OP::popscope:
    case OP::nextname:
    case OP::hasnext:
    case OP::pushnull:
    case OP::pushundefined:
    case OP::nextvalue:
    case OP::pushtrue:
    case OP::pushfalse:
    case OP::pushnan:
    case OP::pop:
    case OP::dup:
    case OP::swap:
    case OP::pushscope:
    case OP::li8:
    case OP::li16:
    case OP::li32:
    case OP::lf32:
    case OP::lf64:
    case OP::si8:
    case OP::si16:
    case OP::si32:
    case OP::sf32:
    case OP::sf64:
    case OP::returnvoid:
    case OP::returnvalue:
    case OP::sxi1:
    case OP::sxi8:
    case OP::sxi16:
    case OP::newactivation:
    case OP::getglobalscope:
    case OP::convert_s:
    case OP::esc_xelem:
    case OP::esc_xattr:
    case OP::convert_i:
    case OP::convert_u:
    case OP::convert_d:
    case OP::convert_b:
    case OP::convert_o:
    case OP::checkfilter:
    case OP::convert_f:
    case OP::unplus:
    case OP::convert_f4:
    case OP::coerce_b:
    case OP::coerce_a:
    case OP::coerce_i:
    case OP::coerce_d:
    case OP::coerce_s:
    case OP::astypelate:
    case OP::coerce_u:
    case OP::coerce_o:
    case OP::negate:
    case OP::increment:
    case OP::decrement:
    case OP::OP_typeof:
    case OP::OP_not:
    case OP::bitnot:
    case OP::add:
    case OP::subtract:
    case OP::multiply:
    case OP::divide:
    case OP::modulo:
    case OP::lshift:
    case OP::rshift:
    case OP::urshift:
    case OP::OP_bitand:
    case OP::OP_bitor:
    case OP::bitxor:
    case OP::equals:
    case OP::strictequals:
    case OP::lessthan:
    case OP::lessequals:
    case OP::greaterthan:
    case OP::greaterequals:
    case OP::OP_instanceof:
    case OP::istypelate:
    case OP::in:
    case OP::increment_i:
    case OP::decrement_i:
    case OP::negate_i:
    case OP::add_i:
    case OP::subtract_i:
    case OP::multiply_i:
    case OP::getlocal0:
    case OP::getlocal1:
    case OP::getlocal2:
    case OP::getlocal3:
    case OP::setlocal0:
    case OP::setlocal1:
    case OP::setlocal2:
    case OP::setlocal3:
        break; // empty
    case OP::pushbyte:
        ins->args.push_back(static_cast<uint32_t>(stream.readU8()));
        break;
    case OP::getsuper:
    case OP::setsuper:
    case OP::dxns:
    case OP::kill:
    case OP::pushfloat:
    case OP::pushshort:
    case OP::pushstring:
    case OP::pushint:
    case OP::pushuint:
    case OP::pushdouble:
    case OP::pushnamespace:
    case OP::newfunction:
    case OP::call:
    case OP::construct:
    case OP::constructsuper:
    case OP::applytype:
    case OP::pushfloat4:
    case OP::newobject:
    case OP::newarray:
    case OP::newclass:
    case OP::getdescendants:
    case OP::newcatch:
    case OP::findpropstrict:
    case OP::findproperty:
    case OP::finddef:
    case OP::getlex:
    case OP::setproperty:
    case OP::getlocal:
    case OP::setlocal:
    case OP::getscopeobject:
    case OP::getproperty:
    case OP::getouterscope:
    case OP::initproperty:
    case OP::deleteproperty:
    case OP::getslot:
    case OP::setslot:
    case OP::getglobalslot:
    case OP::setglobalslot:
    case OP::coerce:
    case OP::astype:
    case OP::inclocal:
    case OP::declocal:
    case OP::istype:
    case OP::inclocal_i:
    case OP::declocal_i:
    case OP::debugline:
    case OP::debugfile:
    case OP::bkptline:
        ins->args.push_back(stream.readU30());
        break;
    case OP::hasnext2:
    case OP::callmethod:
    case OP::callstatic:
    case OP::callsuper:
    case OP::callproperty:
    case OP::constructprop:
    case OP::callproplex:
    case OP::callsupervoid:
    case OP::callpropvoid:
        ins->args.push_back(stream.readU30());
        ins->args.push_back(stream.readU30());
        break;
    case OP::ifnlt:
    case OP::ifnle:
    case OP::ifngt:
    case OP::ifnge:
    case OP::jump:
    case OP::iftrue:
    case OP::iffalse:
    case OP::ifeq:
    case OP::ifne:
    case OP::iflt:
    case OP::ifle:
    case OP::ifgt:
    case OP::ifge:
    case OP::ifstricteq:
    case OP::ifstrictne:
        target = stream.readS24() + stream.pos();
        jump_targets.emplace(target, std::make_pair(ins.get(), 0));
        ins->args.push_back(target);
        ins->targets.emplace_back();
        break;
    case OP::lookupswitch: {
        // The base address for the lookupswitch's targets is always the instruction's address unlike all other jump
        // instructions.
        target = addr + stream.readS24();
        jump_targets.emplace(target, std::make_pair(ins.get(), 0));
        ins->args.push_back(target);
        ins->targets.emplace_back();

        uint32_t count = stream.readU30() + 1;
        for (auto i = 1; i <= count; ++i) {
            target = addr + stream.readS24();
            jump_targets.emplace(target, std::make_pair(ins.get(), i));
            ins->args.push_back(target);
            ins->targets.emplace_back();
        }

        break;
    }
    case OP::debug:
        ins->args.push_back(static_cast<uint32_t>(stream.readU8()));
        ins->args.push_back(stream.readU30());
        ins->args.push_back(static_cast<uint32_t>(stream.readU8()));
        ins->args.push_back(stream.readU30());
        break;
    default:
        throw std::runtime_error(std::string("Unknown instruction arguments: ") + opnames[(uint8_t)opcode]);
        break;
    }
    return ins;
}
}