#pragma once
#include "stream.h"

namespace swf {
/* Utility class to read bits from a StreamReader.
 */
class BitStreamReader {
public:
    /* Create a new BitStreamReader from a StreamReader */
    BitStreamReader(StreamReader* stream) : stream(stream), bitpos(0) {};
    ~BitStreamReader();

    /* Read n bits and return it as an unsigned integer. */
    uint32_t readUB(uint8_t nBits);
    /* Read n bits and return it as a signed integer. */
    int32_t readSB(uint8_t nBits);

private:
    StreamReader* stream;
    uint8_t bitpos; // current position in the byte
};

/* Utility class to write bits into a StreamWrite.
 */
class BitStreamWriter {
public:
    BitStreamWriter(StreamWriter* stream) : stream(stream), bitpos(0), byte(0) {};
    ~BitStreamWriter();

    /* Write an unsigned integer into n bits. */
    void writeUB(uint8_t nBits, uint32_t value);
    /* Write a signed integer into n bits. */
    void writeSB(uint8_t nBits, int32_t value);

    /* Compute the number of bits required to write an unsigned integer. */
    static const uint8_t calcBits(uint32_t value);
    /* Compute the number of bits required to write a signed integer. */
    static const uint8_t calcBits(int32_t value);

private:
    StreamWriter* stream;
    uint8_t bitpos; // current position in the byte
    uint8_t byte; // current byte value
};
}