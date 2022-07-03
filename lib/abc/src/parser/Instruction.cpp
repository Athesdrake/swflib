#include "abc/parser/Instruction.hpp"
#include <cmath>
#include <stdexcept>

namespace swf::abc::parser {
Instruction::Instruction(OP opcode, uint32_t addr) : opcode(opcode), addr(addr) { }

bool Instruction::isJump() const { return opcode >= OP::ifnlt && opcode <= OP::lookupswitch; }

void Instruction::write(StreamWriter& stream) {
    addr = static_cast<uint32_t>(stream.size());
    stream.writeU8(static_cast<uint8_t>(opcode));

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
        stream.writeU8(args[0]);
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
        stream.writeU30(args[0]);
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
        stream.writeU30(args[0]);
        stream.writeU30(args[1]);
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
        stream.writeS24(static_cast<int32_t>(args[0]) - stream.size() - 3);
        break;
    case OP::lookupswitch: {
        // The base address for the lookupswitch's targets is always the instruction's address unlike all other jump
        // instructions.
        stream.writeS24(static_cast<int32_t>(args[0]) - addr);
        stream.writeU30(args.size() - 2);
        for (auto i = 1; i < args.size(); ++i)
            stream.writeS24(args[i] - addr);

        break;
    }
    case OP::debug:
        stream.writeU8(args[0]);
        stream.writeU30(args[1]);
        stream.writeU8(args[2]);
        stream.writeU30(args[3]);
        break;
    default:
        throw std::runtime_error(std::string("Unknown instruction arguments: ") + opnames[(uint8_t)opcode]);
        break;
    }
}

const uint32_t getU30size(uint32_t value) { return std::max(1.0, std::ceil(std::log2(value + 1) / 7)); }

uint32_t Instruction::size() {
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
        return 1;
    case OP::pushbyte:
        return 2;
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
        return getU30size(args[0]) + 1;
    case OP::hasnext2:
    case OP::callmethod:
    case OP::callstatic:
    case OP::callsuper:
    case OP::callproperty:
    case OP::constructprop:
    case OP::callproplex:
    case OP::callsupervoid:
    case OP::callpropvoid:
        return getU30size(args[0]) + getU30size(args[1]) + 1;
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
        return 4;
    case OP::lookupswitch:
        return 1 + getU30size(args.size() - 2) + 3 * args.size();
    case OP::debug:
        return 3 + getU30size(args[1]) + getU30size(args[3]);
    default:
        throw std::runtime_error(std::string("Unknown instruction arguments: ") + opnames[(uint8_t)opcode]);
    }
}
}