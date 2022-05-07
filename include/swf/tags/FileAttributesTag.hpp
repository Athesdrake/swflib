#pragma once
#include "Tag.hpp"

namespace swf {
/* Attributes value */
enum FileAttributes {
    UseNetwork    = 0x01,
    ActionScipt3  = 0x08,
    HasMetadata   = 0x10,
    UseGPU        = 0x20,
    UseDirectBlit = 0x40,
};

/* If used, it must be the first tag.
   It defines differents options to configure the Flash Player.
 */
class FileAttributesTag : public ITag<FileAttributesTag> {
private:
    uint8_t flags = 0x69; /* UseDirectBlit | UseGPU | ActionScript3 | UseNetwork */

public:
    static std::shared_ptr<Tag> create() { return std::make_shared<FileAttributesTag>(); }
    static inline const TagID tagId = TagID::FileAttributes;

    const std::string getTagName() override { return "FileAttributesTag"; };

    FileAttributesTag() { }

    void read(StreamReader&& stream) override;
    void write(StreamWriter& stream) override;

    /* Tells the Flash Player to use hardware acceleration to blit graphics to the screen if available */
    bool useDirectBlit() const;
    /* Tells the Player to use GPU compositing features if available */
    bool useGPU() const;
    /* Defines if the SWF has metadata */
    bool hasMetadata() const;
    /* Defines if the SWF contains AVM2 code */
    bool useAS3() const;
    /* Gives network access if the file is run on a local drive */
    bool useNetwork() const;

    /* Tells the Flash Player to use hardware acceleration to blit graphics to the screen if available */
    void setUseDirectBlit(bool use);
    /* Tells the Player to use GPU compositing features if available */
    void setUseGPU(bool use);
    /* Defines if the SWF has metadata */
    void setHasMetadata(bool use);
    /* Defines if the SWF contains AVM2 code */
    void setUseAS3(bool use);
    /* Gives network access if the file is run on a local drive */
    void setUseNetwork(bool use);

private:
    void setBit(FileAttributes bit, bool enable);
};
}