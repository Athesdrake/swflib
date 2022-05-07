#pragma once
#include "Tag.hpp"

namespace swf {
/* Describe the SWF file to an external process.
   Used by search engine to access data from the file.
 */
class MetadataTag : public ITag<MetadataTag> {
public:
    static inline const TagID tagId = TagID::Metadata;
    static std::shared_ptr<Tag> create() { return std::make_shared<MetadataTag>(); }

    const std::string getTagName() override { return "MetadataTag"; };

    std::string metadata; // Metadata as XML

    MetadataTag() { }

    void read(StreamReader&& stream) override;
    void write(StreamWriter& stream) override;
};
}