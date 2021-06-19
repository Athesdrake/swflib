#include "swflib.h"
#include <cmath>
#include <iostream>
#include <zlib.h>
//#include "chrono.h"

#include "bitstream.h"

namespace swf {
Swf::Swf() {
    framesize    = Rect();
    signature    = { 'F', 'W', 'S' };
    symbol_class = nullptr;
}

void Swf::read_header(StreamReader& stream) {
    for (auto& c : signature)
        c = stream.readU8();

    if (!isValidCompression(signature[0]))
        throw UnsupportedCompression(signature[0]);

    if (signature[1] != 'W' || signature[2] != 'S')
        throw InvalidSignature((char*)signature.data());

    version     = stream.readU8();
    file_length = stream.readU32();
}
void Swf::read(StreamReader& stream) {
    read_header(stream);
    switch (Compression(signature[0])) {
    case Compression::Zlib:
        stream.inflateZlib(file_length - 8);
        if (!stream)
            throw CompressionError("zlib");
        break;
    case Compression::Lzma:
        stream.inflateLzma(file_length - 8);
        if (!stream)
            throw CompressionError("lzma");
        break;
    default:
        break;
    }

    framesize.read(stream);
    framerate  = static_cast<double>(stream.readU8()) / 256.0 + static_cast<double>(stream.readU8());
    framecount = stream.readU16();

    while (true) {
        uint16_t header   = stream.readU16();
        uint16_t tag_type = header >> 6;
        uint32_t length   = header & 0x3F;
        auto registry     = Tag::get_registry();

        if (length == 0x3F)
            length = stream.readU32();

        const uint8_t* data = stream.raw();
        stream.skip(length);

        auto it = registry.find(TagID(tag_type));

        std::shared_ptr<Tag> tag = tags.emplace_back(it == registry.end() ? UnknownTag::create() : it->second());

        tag->read(StreamReader { data, data + length, false });

        switch (tag->getId()) {
        case TagID::Unknown:
            std::dynamic_pointer_cast<UnknownTag>(tag)->id = tag_type;
            break;
        case TagID::DefineBinaryData:
            binaries.emplace_back(std::dynamic_pointer_cast<DefineBinaryDataTag>(tag).get());
            break;
        case TagID::SymbolClass:
            symbol_class = std::dynamic_pointer_cast<SymbolClassTag>(tag).get();
            break;
        case TagID::DoABC: {
            auto abc = std::dynamic_pointer_cast<DoABCTag>(tag).get();
            abcfiles.emplace(abc->name, abc);
            break;
        }
        default:
            break;
        }

        if (tag_type == 0)
            break;
    }
}

void Swf::write(StreamWriter& stream) {
    stream.extend(signature.size(), signature.data());
    stream.writeU8(version);

    StreamWriter content;
    framesize.write(content);

    double intpart  = 0;
    double fraction = std::modf(framerate, &intpart);

    content.writeU8(uint8_t(fraction * 256.0));
    content.writeU8(uint8_t(intpart));
    content.writeU16(framecount);

    for (auto tag : tags) {
        StreamWriter stag;
        tag->write(stag);
        size_t length = stag.size();
        uint16_t id   = uint16_t(tag->getId());

        if (id == uint16_t(TagID::Unknown))
            id = std::dynamic_pointer_cast<UnknownTag>(tag)->id;

        content.writeU16(id << 6 | uint16_t(length >= 0x3F ? 0x3F : length));
        if (length >= 0x3F)
            content.writeU32((uint32_t)length);

        content.write(stag);
    }
    stream.writeU32((uint32_t)content.size() + 8);

    switch (Compression(signature[0])) {
    case Compression::Zlib:
        content.deflateZlibInto(stream);
        break;
    case Compression::Lzma:
        content.deflateLzmaInto(stream);
        break;
    case Compression::None:
        stream.write(content);
        break;
    default:
        throw UnsupportedCompression(signature[0]);
    }
}

bool Swf::isCompressed() { return isValidCompression(signature[0]); }
}