#include "bitstream.hpp"
#include "test.hpp"
#include <cmath>
#include <functional>

void test_bitstream_reader(swf::StreamReader* strm) {
    auto stream = swf::BitStreamReader(strm);
    uint32_t size;

    size = stream.readUB(5);
    assert_equal(size, 13);
    assert_equal(stream.readSB(size), -2573);

    size = stream.readUB(5);
    assert_equal(size, 6);
    assert_equal(stream.readUB(size), 43);
}

void test_bitstream_writer(swf::StreamWriter* strm) {
    auto stream = swf::BitStreamWriter(strm);
    int32_t a   = -2573;
    uint32_t b  = 43;

    auto size = swf::BitStreamWriter::calcBits(a);
    stream.writeUB(5, size);
    stream.writeSB(size, a);

    size = swf::BitStreamWriter::calcBits(b);
    stream.writeUB(5, size);
    stream.writeUB(size, b);
}

void benchmark_calcbit() {
    const auto fastlog = static_cast<const uint8_t (*)(uint32_t)>(swf::BitStreamWriter::calcBits);

    auto x = rand();
    bench(fastlog(x), 1000000);
    bench(std::ceil(std::log2(x)), 1000000);

    // ceil(log2(0 or 1)) are both incorrect, so we skip it
    for (uint16_t i = 2; i; ++i)
        assert_equal((int)fastlog(i), std::ceil(std::log2(i)), i);
}

int main() {
    swf::StreamWriter writer;
    test_bitstream_writer(&writer);

    auto buffer = writer.get_buffer();
    auto length = writer.size();

    std::cout << hex(buffer, length) << '\n';

    swf::StreamReader reader(buffer, buffer + length, false);
    test_bitstream_reader(&reader);
    benchmark_calcbit();

    return 0;
}