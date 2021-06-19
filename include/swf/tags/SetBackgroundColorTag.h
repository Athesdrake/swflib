#pragma once
#include "Tag.h"

namespace swf {
/* Set the background color of the display.
 */
class SetBackgroundColorTag : public ITag<SetBackgroundColorTag> {
public:
    static inline const TagID tagId = TagID::SetBackgroundColor;
    static std::shared_ptr<Tag> create() { return std::make_shared<SetBackgroundColorTag>(); }

    const std::string getTagName() override { return "SetBackgroundColorTag"; };

    /* Color of the background display */
    RGB color { 0, 0, 0 };

    SetBackgroundColorTag() { }

    void read(StreamReader&& stream) override;
    void write(StreamWriter& stream) override;
};
}