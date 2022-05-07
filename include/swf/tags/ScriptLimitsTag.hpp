#pragma once
#include "Tag.hpp"

namespace swf {
/* Settings for the script limits in the AVM2.
 */
class ScriptLimitsTag : public ITag<ScriptLimitsTag> {
public:
    static inline const TagID tagId = TagID::ScriptLimits;
    static std::shared_ptr<Tag> create() { return std::make_shared<ScriptLimitsTag>(); }

    const std::string getTagName() override { return "ScriptLimitsTag"; };

    uint16_t max_recursion_depth = 256; // Set the max recursion limit. Default is 256
    // Set the maximum number of seconds the player should process ActionScript before asking if the script should be
    // stopped. The default value varies by platform and is between 15 and 20 seconds.
    uint16_t script_timeout_seconds = 20;

    ScriptLimitsTag() { }

    void read(StreamReader&& stream) override;
    void write(StreamWriter& stream) override;
};
}