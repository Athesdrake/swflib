#pragma once
#include "Tag.h"
#include <unordered_map>

namespace swf {
/* Create an association between symbols in the SWF and ActionScript classes.
   If the character id is zero, the class is assiociated with the main timeline.
   It also means this class is the root class of the SWF.
 */
class SymbolClassTag : public ITag<SymbolClassTag> {
public:
    static inline const TagID tagId = TagID::SymbolClass;
    static std::shared_ptr<Tag> create() { return std::make_shared<SymbolClassTag>(); }

    const std::string getTagName() override { return "ProductInfoTag"; };

    /* Symbols: <Character id>:<Fully-qualified name of the class> */
    std::unordered_map<uint16_t, std::string> symbols;

    SymbolClassTag() { }

    /* Returns the character id from its class' name. */
    uint16_t getCharId(std::string name);

    void read(StreamReader&& stream) override;
    void write(StreamWriter& stream) override;
};
}