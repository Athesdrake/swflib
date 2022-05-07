#pragma once
#include "Tag.hpp"

namespace swf {
/* Embed information about the product that were used to compile the SWF file.
   http://wahlers.com.br/claus/blog/undocumented-swf-tags-written-by-mxmlc/
 */
class ProductInfoTag : public ITag<ProductInfoTag> {
public:
    static inline const TagID tagId = TagID::ProductInfo;
    static std::shared_ptr<Tag> create() { return std::make_shared<ProductInfoTag>(); }

    const std::string getTagName() override { return "ProductInfoTag"; };

    /* The product identifier. Known identifiers:
        - 0: Unknown
        - 1: Macromedia Flex for J2EE
        - 2: Macromedia Flex for .NET
        - 3: Adobe Flex
     */
    uint32_t productId = 0;
    /* The product edition. Known editions:
        - 0: Developer Edition
        - 1: Full Commercial Edition
        - 2: Non Commercial Edition
        - 3: Educational Edition
        - 4: Not For Resale (NFR) Edition
        - 5: Trial Edition
        - 6: None
    */
    uint32_t edition = 0;
    /* The product version */
    struct Version {
        uint8_t major = 0;
        uint8_t minor = 1;
    } version;
    uint64_t build       = 0; // Extended information about the product's version
    uint64_t compileDate = 0; // Milliseconds since 1st January 1970

    ProductInfoTag() { }

    void read(StreamReader&& stream) override;
    void write(StreamWriter& stream) override;
};
}