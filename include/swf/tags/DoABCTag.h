#pragma once
#include "Tag.h"
#include "abc/AbcFile.h"

namespace swf {
/* Defines the code to be executed in the AVM2.
 */
class DoABCTag : public ITag<DoABCTag> {
public:
    static inline const TagID tagId = TagID::DoABC;
    static std::shared_ptr<Tag> create() { return std::make_shared<DoABCTag>(); }

    const std::string getTagName() override { return "DoABCTag"; };

    std::shared_ptr<abc::AbcFile> abcfile; // Holds the parsed bytecode
    bool is_lazy = false; // Indicates if the code should be executed immediatly after being parsed or not
    std::string name; // The name assigned to the bytecode

    DoABCTag() { }

    void read(StreamReader&& stream) override;
    void write(StreamWriter& stream) override;
};
}