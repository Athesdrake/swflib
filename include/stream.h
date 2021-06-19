#pragma once
#include <algorithm>
#include <memory>
#include <stdint.h>
#include <string>
#include <vector>

namespace swf {
class BitStreamReader;
class BitStreamWriter;
class StreamWriter;

/* Despite its name, it is not a stream.
   It holds a binary data in memory and provides methods to read and extract values from its buffer.
   The dtor will delete the buffer, if marked as managed.
 */
class StreamReader {
    friend class BitStreamReader;
    friend class StreamWriter;

private:
    bool managed; // If true, the buffer will be deleted in the destructor
    const uint8_t* buffer; // The start of the buffer
    const uint8_t* end_buffer; // The end of the buffer

protected:
    uint8_t* ptr; // Current position in the buffer

public:
    /* Creates a new StreamReader without any data. */
    StreamReader() : StreamReader((uint8_t*)nullptr, (uint8_t*)nullptr, false) {};
    /* Creates a new StreamReader (managed by default) from the buffer. */
    StreamReader(uint8_t* buffer, uint8_t* end, bool managed = true)
        : StreamReader((const uint8_t*)buffer, (const uint8_t*)end, managed) { }
    /* Creates a new StreamReader (managed by default) from the buffer. */
    StreamReader(const uint8_t* buffer, const uint8_t* end, bool managed = true);
    ~StreamReader();

    /* Creates a new StreamReader from the content of a file. It supports windows API to read data faster. */
    static std::unique_ptr<StreamReader> fromfile(std::string filename);

    /* Tells if the end of the buffer has been reached. */
    bool end();
    /* Return false if the current position is out of bound. */
    bool valid() const;
    /* Return the remaining bytes in the buffer. */
    size_t remaining() const;
    /* Return the total size of the buffer. */
    size_t size() const;
    /* Reset the position to the start of the buffer. */
    void reset();
    /* Skip n bytes. */
    void skip(uint32_t length);
    /* Skip n variable-length int(s). */
    void skip30(uint32_t count = 1);

    /* Return false if the current position is out of bound. */
    operator bool() const;
    /* Move a buffer from a StreamReader to another. */
    StreamReader& operator=(StreamReader&& stream) noexcept;

    /* Return the current position in the buffer. */
    const uint8_t* raw() { return ptr; }
    /* Return the end position of the buffer. */
    const uint8_t* endpos() { return end_buffer; }

    /* Copy n bytes into an uint8_t c-array. */
    void copy(uint32_t length, uint8_t* target);
    /* Copy n bytes into an uint8_t vector. */
    void copy(uint32_t length, std::vector<uint8_t>& target);
    /* Copy n bytes into a string. */
    void copy(uint32_t length, std::string& target);

    /* Read an arbitrary type from the buffer. */
    template <typename T> T read() {
        T value = *reinterpret_cast<T*>(ptr);
        ptr += sizeof(T);
        return value;
    }

    uint8_t readU8() { return read<uint8_t>(); }
    uint16_t readU16() { return read<uint16_t>(); }
    uint32_t readU32() { return read<uint32_t>(); }
    uint64_t readU64() { return read<uint64_t>(); }
    int8_t readS8() { return static_cast<int8_t>(readU8()); }
    int16_t readS16() { return static_cast<int16_t>(readU16()); }
    int32_t readS32() { return static_cast<int32_t>(readU32()); }
    int64_t readS64() { return static_cast<int64_t>(readU64()); }
    float readFloat() { return read<float>(); }
    double readDouble() { return read<double>(); }

    /* Read a 24-bit unsigned integer. */
    uint32_t readU24();
    /* Read a variable-length unsigned integer. See https://en.wikipedia.org/wiki/LEB128 for more informations. */
    uint32_t readU30();
    /* Read a 24-bit signed integer. */
    int32_t readS24();
    /* Read a variable-length signed integer. See https://en.wikipedia.org/wiki/LEB128 for more informations. */
    int32_t readS30() { return static_cast<int32_t>(readU30()); }

    /* Read a null-terminated char array. */
    std::string readCharArray();
    /* Read a string preceded with a U30 integer defining the string's length. */
    std::string readString();
    /* Same as readString but return a string_view and therefore uses less memory. */
    std::string_view readStringView();

    /* Inflate the next n bytes using zlib. The internal buffer will be deleted (if managed) and replaced by the
       inflated data. Hence the data beyond these n bytes will be lost.
    */
    void inflateZlib(size_t size);
    /* Same as inflateZlib but uses LZMA. */
    void inflateLzma(size_t size);
};

/* Despite its name, it is not a stream.
   It holds a binary data in memory and provides methods to write and insert values into its buffer.
   The dtor will delete the internal buffer.
 */
class StreamWriter {
    friend class BitStreamWriter;

private:
    std::vector<uint8_t> buffer;

public:
    /* Creates a new StreamWriter. */
    StreamWriter();
    ~StreamWriter();

    /* Write the buffer to a file. It supports windows API to write data faster. */
    bool tofile(std::string filename);
    /* Write the buffer to a file. It supports windows API to write data faster. */
    bool tofile(const char* filename) { return tofile(std::string(filename)); };

    /* Write the buffer of another stream into its own buffer. */
    void write(StreamReader& stream);
    /* Write the buffer of another stream into its own buffer. */
    void write(StreamWriter& stream);
    /* Copy n bytes into the internal buffer from an uint8_t c-array. */
    void extend(uint32_t length, uint8_t* source);
    /* Copy n bytes into the internal buffer from an uint8_t vector. */
    void extend(uint32_t length, std::vector<uint8_t> source);

    /* Return the current size of the buffer. */
    size_t size() const;
    /* Return the internal buffer. Unsafe. Can lead to dangling pointer. */
    const uint8_t* get_buffer() const { return &buffer[0]; }

    /* Write an arbitrary type into the buffer. */
    template <typename T> void write(T value) { extend(sizeof(T), reinterpret_cast<uint8_t*>(&value)); }
    /* Write an arbitrary type into the buffer at a specific index. */
    template <typename T> void write_at(T value, size_t index) {
        uint8_t* data = reinterpret_cast<uint8_t*>(&value);
        std::copy(data, data + sizeof(T), buffer.begin() + index);
    }

    void writeU8(uint8_t value) { write<uint8_t>(value); }
    void writeU16(uint16_t value) { write<uint16_t>(value); }
    void writeU32(uint32_t value) { write<uint32_t>(value); }
    void writeU64(uint64_t value) { write<uint64_t>(value); }
    void writeS8(int8_t value) { writeU8(static_cast<uint8_t>(value)); }
    void writeS16(int16_t value) { writeU16(static_cast<uint16_t>(value)); }
    void writeS32(int32_t value) { writeU32(static_cast<uint32_t>(value)); }
    void writeS64(int64_t value) { writeU64(static_cast<uint64_t>(value)); }
    void writeFloat(float value) { write<float>(value); }
    void writeDouble(double value) { write<double>(value); }

    /* Write a 24-bit unsigned integer. */
    void writeU24(uint32_t value);
    /* Write a variable-length unsigned integer. See https://en.wikipedia.org/wiki/LEB128 for more informations. */
    void writeU30(uint32_t value);
    /* Write a 24-bit signed integer. */
    void writeS24(int32_t value) { writeU24(static_cast<uint32_t>(value)); }
    /* Write a variable-length signed integer. See https://en.wikipedia.org/wiki/LEB128 for more informations. */
    void writeS30(int32_t value) { writeU30(static_cast<uint32_t>(value)); }

    /* Write a null-terminated char array. */
    void writeCharArray(std::string value);
    /* Write a string preceded with a U30 integer defining the string's length. */
    void writeString(std::string value);
    /* Same as writeString but takes a string_view. */
    void writeStringView(std::string_view value);

    /* Deflate the internal buffer into the given buffer using zlib. */
    size_t deflateZlib(std::vector<uint8_t>& buffer, size_t start, size_t length, size_t offset = 0);
    /* Deflate the internal buffer into the given stream using zlib. */
    size_t deflateZlibInto(StreamWriter& stream) { return deflateZlib(stream.buffer, 0, size(), stream.buffer.size()); }
    /* Deflate the internal buffer into the given buffer using LZMA. */
    size_t deflateLzma(std::vector<uint8_t>& buffer, size_t start, size_t length, size_t offset = 0);
    /* Deflate the internal buffer into the given stream using LZMA. */
    size_t deflateLzmaInto(StreamWriter& stream) { return deflateLzma(stream.buffer, 0, size(), stream.buffer.size()); }
};
}