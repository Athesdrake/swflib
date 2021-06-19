#pragma once
#include <array>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "abc/AbcFile.h"
#include "datatypes.h"
#include "errors.h"
#include "stream.h"
#include "tags/tags.h"

namespace swf {

/* Supported compressions by the library.
 */
enum class Compression : uint8_t {
    None = 'F', // no compression
    Zlib = 'C', // zlib compression
    Lzma = 'Z', // lzma compression
};

/* Check if a compression is valid and supported. */
constexpr bool isValidCompression(Compression c) {
    for (auto ch : { Compression::None, Compression::Zlib, Compression::Lzma })
        if (c == ch)
            return true;

    return false;
}
/* Check if a compression is valid and supported. */
constexpr bool isValidCompression(const char c) { return isValidCompression(Compression(c)); }

/* Represent a ShockWave Flash file.
 */
class Swf {
public:
    std::array<uint8_t, 3> signature; // SWF's signature. The first character must be a valid compression. The two
                                      // others must be 'W' and 'S'.
    uint8_t version      = 14; // SWF's version
    uint32_t file_length = 0; // The (uncompressed) file's total length
    double framerate     = 0.0; // SWF's framerate
    uint16_t framecount  = 0; // The total number of frames
    Rect framesize;

    std::vector<std::shared_ptr<Tag>> tags; // Contains all tags
    SymbolClassTag* symbol_class;
    std::vector<DefineBinaryDataTag*> binaries; // List of all binary data
    std::unordered_map<std::string, DoABCTag*> abcfiles; // Map of all abc tags

    Swf();

    /* Validate the 8-bytes header and return the file's size. */
    void read_header(StreamReader& stream);
    void read(StreamReader& stream);
    void write(StreamWriter& stream);

    /* Returns wether or not the file is compressed. */
    bool isCompressed();
};
}