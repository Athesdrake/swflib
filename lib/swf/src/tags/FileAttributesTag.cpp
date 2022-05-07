#include "swf/tags/FileAttributesTag.hpp"

namespace swf {
void FileAttributesTag::read(StreamReader&& stream) {
    flags            = stream.readU8();
    int32_t reserved = stream.readS24();
    (void)reserved;
}
void FileAttributesTag::write(StreamWriter& stream) {
    stream.writeU8(flags);
    stream.writeS24(0);
}

bool FileAttributesTag::useDirectBlit() const { return flags & FileAttributes::UseDirectBlit; }
bool FileAttributesTag::useGPU() const { return flags & FileAttributes::UseGPU; }
bool FileAttributesTag::hasMetadata() const { return flags & FileAttributes::HasMetadata; }
bool FileAttributesTag::useAS3() const { return flags & FileAttributes::ActionScipt3; }
bool FileAttributesTag::useNetwork() const { return flags & FileAttributes::UseNetwork; }

void FileAttributesTag::setUseDirectBlit(bool use) { setBit(FileAttributes::UseDirectBlit, use); }
void FileAttributesTag::setUseGPU(bool use) { setBit(FileAttributes::UseGPU, use); }
void FileAttributesTag::setHasMetadata(bool use) { setBit(FileAttributes::HasMetadata, use); }
void FileAttributesTag::setUseAS3(bool use) { setBit(FileAttributes::ActionScipt3, use); }
void FileAttributesTag::setUseNetwork(bool use) { setBit(FileAttributes::UseNetwork, use); }

void FileAttributesTag::setBit(FileAttributes bit, bool enable) {
    if (enable)
        flags |= bit;
    else
        flags &= ~bit;
}
}