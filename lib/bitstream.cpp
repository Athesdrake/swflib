#include "bitstream.hpp"
#include <cmath>

namespace swf {
BitStreamReader::~BitStreamReader() {
    // padding
    if (bitpos != 0)
        ++stream->ptr;
}

uint32_t BitStreamReader::readUB(uint8_t nBits) {
    if (nBits == 0)
        return 0;

    // read the maximum of bits available
    uint8_t start_bits = std::min(uint8_t(8 - bitpos), nBits);
    uint32_t value     = (*stream->ptr >> (8 - start_bits - bitpos)) & (1 << start_bits) - 1;

    // increment the position by the bits read
    bitpos += start_bits;
    if (bitpos > 7) {
        bitpos = 0;
        ++stream->ptr;
    }

    nBits -= start_bits;
    if (nBits) // shift and read more bits if needed
        return (value << nBits) | readUB(nBits);

    return value;
}

int32_t BitStreamReader::readSB(uint8_t nBits) {
    if (nBits < 2)
        return readUB(nBits);

    int32_t value = static_cast<int32_t>(readUB(nBits));
    int32_t shift = 32 - nBits;

    // Shift to retrieve the value's sign
    return (value << shift) >> shift;
}

BitStreamWriter::~BitStreamWriter() {
    // flush the byte if necessary
    if (bitpos != 0)
        stream->writeU8(byte);
}

void BitStreamWriter::writeUB(uint8_t nBits, uint32_t value) {
    while (nBits > 0) {
        // write the maximum amount of bits
        uint8_t maxbits = std::min(uint8_t(8 - bitpos), nBits);
        uint8_t bits    = ((((value >> (nBits - maxbits)) & (1 << maxbits) - 1)) << (8 - maxbits - bitpos));
        byte |= bits;

        bitpos += maxbits;
        nBits -= maxbits;
        if (bitpos > 7) {
            bitpos = 0;
            stream->writeU8(byte);
            byte = 0;
        }

        if (nBits) // shift the value
            value = value & (1 << nBits) - 1;
    }
}

void BitStreamWriter::writeSB(uint8_t nBits, int32_t value) {
    if (nBits < 2)
        return;

    if (value < 0) // remove the two's complement
        value &= ~(-1 << nBits);

    writeUB(nBits, value);
    // writeUB(1, static_cast<uint32_t>(value < 0));
    // writeUB(nBits - 1, static_cast<uint32_t>(value));
}

const uint8_t BitStreamWriter::calcBits(uint32_t value) {
#ifdef __GNUC__
    // Same as ceil(log2(value)) but significantly faster.
    return (value ? 0x40 - __builtin_clzl(value - 1) : 0);
#else
    return std::ceil(std::log2(value));
#endif
    // TODO: Support MSCV intrinsics (_BitScanReverse?)
    // TODO: Detect if compiler has __builtin_clzl
}
const uint8_t BitStreamWriter::calcBits(int32_t value) { return calcBits(static_cast<uint32_t>(std::abs(value))) + 1; }
}