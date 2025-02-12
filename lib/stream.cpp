#include "stream.hpp"
#include "errors.hpp"

#include <fstream>
#include <lzma.h>
#include <zlib.h>

namespace swf {
StreamReader::StreamReader(const uint8_t* buffer, const uint8_t* end, bool managed) : managed(managed) {
    this->buffer     = buffer;
    this->ptr        = (uint8_t*)buffer;
    this->end_buffer = end;
}
StreamReader::~StreamReader() {
    if (managed && buffer != nullptr) {
        delete[] buffer;
        buffer = nullptr;
    }
}

std::unique_ptr<StreamReader> StreamReader::fromfile(std::string filename) {
    std::ifstream file(filename, std::ifstream::ate | std::ifstream::binary);
    if (file.fail())
        throw std::runtime_error("No such file or directory: '" + filename + "'");

    size_t size = (size_t)file.tellg();
    file.seekg(0, std::ios::beg);
    uint8_t* buffer = new uint8_t[size];

    file.read((char*)buffer, size);
    file.close();
    return std::make_unique<StreamReader>(buffer, buffer + size);
}

bool StreamReader::end() { return ptr == end_buffer; }

bool StreamReader::valid() const { return buffer != nullptr && ptr < end_buffer; }

size_t StreamReader::size() const { return end_buffer - buffer; }
size_t StreamReader::remaining() const { return end_buffer - ptr; }
size_t StreamReader::pos() const { return ptr - buffer; }

StreamReader::operator bool() const { return valid(); }

StreamReader& StreamReader::operator=(StreamReader&& stream) noexcept {
    ptr        = stream.ptr;
    buffer     = stream.buffer;
    end_buffer = stream.end_buffer;

    stream.buffer     = nullptr;
    stream.ptr        = nullptr;
    stream.end_buffer = nullptr;
    return *this;
}

void StreamReader::reset() { ptr = (uint8_t*)buffer; }

void StreamReader::skip(uint32_t length) { ptr += length; }
void StreamReader::skip30(uint32_t count) {
    while (count-- > 0) {
        if (!(ptr[0] & 0x80)) {
            ptr += 1;
            continue;
        }
        if (!(ptr[1] & 0x80)) {
            ptr += 2;
            continue;
        }
        if (!(ptr[2] & 0x80)) {
            ptr += 3;
            continue;
        }
        if (!(ptr[3] & 0x80)) {
            ptr += 4;
            continue;
        }
        // if (!(*p[4] & 0x80)) { p += 5; continue; }    // test should be unnecessary
        ptr += 5;
    }
}

void StreamReader::copy(uint32_t length, uint8_t* target) {
    std::copy(ptr, ptr + length, target);
    ptr += length;
}
void StreamReader::copy(uint32_t length, std::vector<uint8_t>& target) {
    target.assign(ptr, ptr + length);
    ptr += length;
}
void StreamReader::copy(uint32_t length, std::string& target) {
    target.assign(ptr, ptr + length);
    ptr += length;
}

uint32_t StreamReader::readU24() {
    uint32_t value = ptr[0] | (ptr[1] << 8) | (ptr[2] << 16);
    ptr += 3;
    return value;
}
uint32_t StreamReader::readU30() {
    uint32_t value = 0;
    uint8_t byte;

    for (int i = 0; i < 35; i += 7) {
        byte = *ptr;
        ptr++;
        value += (byte & 0x7f) << i;
        if (!(byte & 0x80))
            break;
    }

    return value;
}
int32_t StreamReader::readS24() {
    int32_t value = ptr[0] | (ptr[1] << 8) | (int8_t(ptr[2]) << 16);
    ptr += 3;
    return value;
}

std::string StreamReader::readCharArray() {
    std::string value((char*)ptr);
    ptr += value.size() + 1;

    return value;
}
std::string StreamReader::readString() {
    uint32_t length = readU30();
    std::string value((char*)ptr, length);
    ptr += length;

    return value;
}
std::string_view StreamReader::readStringView() {
    uint32_t length = readU30();
    std::string_view value((char*)ptr, length);
    ptr += length;

    return value;
}

void StreamReader::inflateZlib(size_t size) {
    z_stream strm {};

    if (inflateInit(&strm) == Z_OK) {
        char* buffer = new char[size];
        char* ptr    = buffer;
        int rcode;

        strm.avail_in = (uInt)(this->end_buffer - this->ptr);
        strm.next_in  = (Bytef*)this->ptr;

        do {
            strm.avail_out = (uInt)size;
            strm.next_out  = (Bytef*)ptr;
            rcode          = inflate(&strm, Z_NO_FLUSH);

            if (strm.total_out > size) {
                ptr = buffer + strm.total_out;
                size -= strm.total_out;
            }
        } while (rcode == Z_OK);

        if (managed)
            delete[] this->buffer;

        this->buffer     = (uint8_t*)buffer;
        this->managed    = true;
        this->end_buffer = this->buffer + size;
    } else {
        if (managed)
            delete[] this->buffer;
        this->buffer     = nullptr;
        this->end_buffer = nullptr;
    }
    inflateEnd(&strm);
    this->ptr = (uint8_t*)this->buffer;
}

void StreamReader::inflateLzma(size_t size) {
    uint8_t* buffer = new uint8_t[size];
    uint8_t* ptr    = buffer;
    lzma_ret rcode;
    lzma_stream strm = LZMA_STREAM_INIT;

    if ((rcode = lzma_auto_decoder(&strm, UINT32_MAX, LZMA_TELL_UNSUPPORTED_CHECK)) != LZMA_OK)
        throw CompressionError("lzma");

    strm.next_in   = this->ptr;
    strm.avail_in  = this->end_buffer - this->ptr;
    strm.next_out  = ptr;
    strm.avail_out = size;

    rcode = lzma_code(&strm, LZMA_FINISH);
    lzma_end(&strm);
    bool fail = rcode != LZMA_STREAM_END;

    if (managed)
        delete[] this->buffer;

    if (fail) {
        this->buffer     = nullptr;
        this->end_buffer = nullptr;
    } else {

        this->buffer     = buffer;
        this->managed    = true;
        this->end_buffer = this->buffer + size;
    }

    this->ptr = (uint8_t*)this->buffer;
}

StreamWriter::StreamWriter() { buffer = std::vector<uint8_t>(); }

StreamWriter::~StreamWriter() { }

bool StreamWriter::tofile(std::string filename) {
    std::ofstream file(filename, std::ifstream::binary);
    if (!file)
        return false;

    file.write((char*)buffer.data(), buffer.size());
    file.close();

    return true;
}

void StreamWriter::write(StreamReader& stream) {
    buffer.reserve(buffer.size() + stream.end_buffer - stream.ptr);
    buffer.insert(buffer.end(), stream.ptr, (uint8_t*)stream.end_buffer);
}

void StreamWriter::write(StreamWriter& stream) {
    buffer.reserve(buffer.size() + stream.size());
    buffer.insert(buffer.end(), stream.buffer.begin(), stream.buffer.end());
}

void StreamWriter::extend(uint32_t length, uint8_t* source) {
    buffer.reserve(buffer.size() + length);
    buffer.insert(buffer.end(), source, source + length);
}

void StreamWriter::extend(uint32_t length, std::vector<uint8_t> source) {
    buffer.reserve(buffer.size() + length);
    buffer.insert(buffer.end(), source.begin(), source.begin() + length);
}

size_t StreamWriter::size() const { return buffer.size(); }

void StreamWriter::writeU24(uint32_t value) {
    writeU8(value & 0xFF);
    writeU8((value >> 8) & 0xFF);
    writeU8((value >> 16) & 0xFF);
}

void StreamWriter::writeU30(uint32_t value) {
    /*do {
        buffer.push_back((value & 0x7F) | ((value > 0x7F) * 0x80));
        value >>= 7;
    } while (value);*/
    uint8_t byte;
    do {
        byte = static_cast<uint8_t>(value & 0x7F);
        value >>= 7;
        if (value)
            byte |= 0x80;
        buffer.push_back(byte);
    } while (value);
}

void StreamWriter::writeCharArray(std::string value) { extend((uint32_t)value.size() + 1, (uint8_t*)value.c_str()); }
void StreamWriter::writeString(std::string value) {
    writeU30((uint32_t)value.size());
    extend((uint32_t)value.size(), (uint8_t*)value.c_str());
}
void StreamWriter::writeStringView(std::string_view value) {
    writeU30((uint32_t)value.size());
    extend((uint32_t)value.size(), (uint8_t*)value.data());
}

size_t StreamWriter::deflateZlib(std::vector<uint8_t>& buffer, size_t start, size_t length, size_t offset) {
    uLong size = compressBound(length);
    buffer.reserve(offset + size);
    buffer.resize(offset + size);
    int rcode = compress2(
        (Bytef*)(buffer.data() + offset), &size, (Bytef*)(this->buffer.data() + start), length, Z_BEST_COMPRESSION);
    if (rcode != Z_OK)
        throw CompressionError("zlib");

    buffer.resize(offset + size);
    return size;
}
size_t StreamWriter::deflateLzma(std::vector<uint8_t>& buffer, size_t start, size_t length, size_t offset) {
    lzma_stream strm = LZMA_STREAM_INIT;
    lzma_ret rcode;
    size_t size = lzma_stream_buffer_bound(length);

    buffer.reserve(offset + size);
    buffer.resize(offset + size);
    if ((rcode = lzma_easy_encoder(&strm, LZMA_MODE_NORMAL, LZMA_CHECK_CRC64)) != LZMA_OK)
        throw CompressionError("lzma");

    strm.next_in   = this->buffer.data() + start;
    strm.avail_in  = length;
    strm.next_out  = buffer.data() + offset;
    strm.avail_out = size;

    rcode = lzma_code(&strm, LZMA_FINISH);
    lzma_end(&strm);
    if (rcode != LZMA_STREAM_END)
        throw CompressionError("lzma");

    buffer.resize(offset + strm.total_out);
    return strm.total_out;
}
}