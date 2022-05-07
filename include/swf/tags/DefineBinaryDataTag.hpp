#pragma once
#include "Tag.hpp"

namespace swf {
/* Represent a binary data embeded inside a SWF file.
   Intended to be used with the SymbolClass tag.
*/
class DefineBinaryDataTag : public ITag<DefineBinaryDataTag> {
private:
    std::unique_ptr<StreamReader> binary;

public:
    static inline const TagID tagId = TagID::DefineBinaryData;
    static std::shared_ptr<Tag> create() { return std::make_shared<DefineBinaryDataTag>(); }

    const std::string getTagName() override { return "DefineBinaryDataTag"; };

    uint16_t charId = 0; // The binary's character id

    DefineBinaryDataTag() { }

    /* Let you get the binary data. */
    StreamReader* getData();

    void read(StreamReader&& stream) override;
    void write(StreamWriter& stream) override;
};
}