#pragma once
#include "Tag.hpp"

namespace swf {
/* Marks the end of the SWF file.
   It is also required to end a sprite definition.
*/
class EndTag : public ITag<EndTag> {
public:
    static inline const TagID tagId = TagID::End;
    static std::shared_ptr<Tag> create() { return std::make_shared<EndTag>(); }

    const std::string getTagName() override { return "EndTag"; };

    EndTag() { }

    void read(StreamReader&& stream) override;
    void write(StreamWriter& stream) override;
};
}