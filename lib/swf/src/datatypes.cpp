#include "swf/datatypes.h"
#include "bitstream.h"
#include <sstream>

namespace swf {
Rect::Rect() {
    min = { 0, 0 };
    max = { 0, 0 };
}

void Rect::read(StreamReader& stream) {
    BitStreamReader bs(&stream);

    uint8_t nBits = static_cast<uint8_t>(bs.readUB(5));

    min.x = bs.readSB(nBits);
    max.x = bs.readSB(nBits);
    min.y = bs.readSB(nBits);
    max.y = bs.readSB(nBits);
}

void Rect::write(StreamWriter& stream) {
    BitStreamWriter bs(&stream);

    uint8_t nBits = std::max({ BitStreamWriter::calcBits(min.x), BitStreamWriter::calcBits(max.x),
        BitStreamWriter::calcBits(min.y), BitStreamWriter::calcBits(max.y) });

    bs.writeUB(5, nBits);
    bs.writeSB(nBits, min.x);
    bs.writeSB(nBits, max.x);
    bs.writeSB(nBits, min.y);
    bs.writeSB(nBits, max.y);
}

std::string Rect::toString() {
    std::stringstream ss;
    ss << "[Rect"
       << " x:" << min.x / TWIPS << " y:" << min.y / TWIPS << " width:" << max.x / TWIPS - min.x / TWIPS
       << " height:" << max.y / TWIPS - min.y / TWIPS << "]";

    return ss.str();
}

void RGB::read(StreamReader& stream) {
    r = stream.readU8();
    g = stream.readU8();
    b = stream.readU8();
}

void RGB::write(StreamWriter& stream) {
    stream.writeU8(r);
    stream.writeU8(g);
    stream.writeU8(b);
}

std::string RGB::toString() {
    std::stringstream ss;
    ss << "RGB(" << r << ", " << g << ", " << b << ")";

    return ss.str();
}

void RGBA::read(StreamReader& stream) {
    RGB::read(stream);
    a = stream.readU8();
}

void RGBA::write(StreamWriter& stream) {
    RGB::write(stream);
    stream.writeU8(a);
}

std::string RGBA::toString() {
    std::stringstream ss;
    ss << "RGBA(" << r << ", " << g << ", " << a << ", " << b << ")";

    return ss.str();
}
}