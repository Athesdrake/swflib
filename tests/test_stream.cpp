#include "swflib.hpp"
#include "test.hpp"
#include <array>

void test_stream_reader(const uint8_t* buffer, size_t length) {
    auto stream = swf::StreamReader(buffer, buffer + length, false);
    stream.inflateZlib(stream.readU32());
    assert_equal(stream.readU8(), 240);
    assert_equal(stream.readU16(), 1278);
    assert_equal(stream.readU24(), 127875);
    assert_equal(stream.readU32(), 75757575);
    assert_equal(stream.readU64(), 78787875757575);

    assert_equal(stream.readS8(), -40);
    assert_equal(stream.readS16(), -278);
    assert_equal(stream.readS24(), -67875);
    assert_equal(stream.readS32(), -5757575);
    assert_equal(stream.readS64(), -78787875757575);

    assert_equal(stream.readFloat(), 24.24f);
    assert_equal(stream.readDouble(), 7545454.2424);

    assert_equal(stream.readU30(), 75423);
    assert_equal(stream.readS30(), -75423);

    assert_equal(stream.readCharArray(), "Null Terminated");
    assert_equal(stream.readString(), "Hi there");
    assert_equal(stream.readStringView(), "Hoi");
}

swf::StreamWriter* test_stream_writer() {
    auto stream = swf::StreamWriter();

    stream.writeU8(240);
    stream.writeU16(1278);
    stream.writeU24(127875);
    stream.writeU32(75757575);
    stream.writeU64(78787875757575);

    stream.writeS8(-40);
    stream.writeS16(-278);
    stream.writeS24(-67875);
    stream.writeS32(-5757575);
    stream.writeS64(-78787875757575);

    stream.writeFloat(24.24f);
    stream.writeDouble(7545454.2424);

    stream.writeU30(75423);
    stream.writeS30(-75423);

    stream.writeCharArray("Null Terminated");
    stream.writeString("Hi there");

    std::string_view vs = "Hoi";
    stream.writeStringView(vs);

    auto result = new swf::StreamWriter();
    result->writeU32(0);
    result->write_at<uint32_t>(static_cast<uint32_t>(stream.deflateZlibInto(*result)), 0);

    return result;
}

int main() {
    swf::StreamWriter* writer = test_stream_writer();
    test_stream_reader(writer->get_buffer(), writer->size());

    delete writer;

    return 0;
}
