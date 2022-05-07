#pragma once
#include "stream.hpp"
#include "swf/datatypes.hpp"
#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>

namespace swf {
enum class TagID : uint16_t {
    End                = 0x00,
    SetBackgroundColor = 0x09,
    ProductInfo        = 0x29,
    ScriptLimits       = 0x41,
    FileAttributes     = 0x45,
    SymbolClass        = 0x4C,
    Metadata           = 0x4D,
    DoABC              = 0x52,
    DefineBinaryData   = 0x57,
    Unknown            = 0x3ff
};

/* The base class of any SWF Tag. Do not inherit directly from it, but from ITag instead.
 */
class Tag {
public:
    using create_func = std::function<std::shared_ptr<Tag>()>;

    /* Returns the tag's id. */
    virtual const TagID getId() = 0;
    /* Returns the tag's name. */
    virtual const std::string getTagName() = 0;

    virtual ~Tag() = default;

    /* Read the data from a stream. */
    virtual void read(StreamReader&& stream) = 0;
    /* Write data to a stream. */
    virtual void write(StreamWriter& stream) = 0;

    /* Internal use to remind you shouldn't inherit directly from Tag. */
    virtual void inherit_from_itag() = 0;

    /* static registry to hold Tags */
    static std::unordered_map<TagID, create_func>& get_registry() {
        static std::unordered_map<TagID, create_func> registry;
        return registry;
    }
};

/* Helper class that lets you add support for a specific SWF Tag
   Several members must be defined:
    - tagId: defines the tag's id. Must be `static inline const`.
    - `static std::shared_ptr<Tag> create() { return std::make_shared<YOUR_TAG>(); }` used to create a new tag.
    - override `getTagName` and return the tag's name.
 */
template <class T> class ITag : public Tag {
public:
    virtual ~ITag() {
        if (!registered)
            std::cerr << "Something went wrong." << std::endl;
    }

    const TagID getId() override { return T::tagId; }

    static inline bool register_type() {
        auto& registry     = get_registry();
        registry[T::tagId] = T::create;
        return true;
    }

    static const bool registered;

protected:
    // Prevent overriding
    virtual void inherit_from_itag() final { }
};

template <class T> const bool ITag<T>::registered = ITag<T>::register_type();
}