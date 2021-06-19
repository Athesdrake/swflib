#pragma once
#include "Tag.h"

namespace swf {
/* Represent an unknown tag, not supported by the library.
 */
class UnknownTag : public ITag<UnknownTag> {
protected:
    StreamReader data;

public:
    static inline const TagID tagId = TagID::Unknown;
    static std::shared_ptr<Tag> create() { return std::make_shared<UnknownTag>(); }

    const std::string getTagName() override { return "UnknownTag"; }

    uint16_t id; // The tag's id

    UnknownTag() { }

    void read(StreamReader&& stream) override;
    void write(StreamWriter& stream) override;
};
}