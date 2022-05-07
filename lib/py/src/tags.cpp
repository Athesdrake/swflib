#include "swf/tags/tags.hpp"
#include "modules.hpp"

// Make a trampoline
class PyTag : public swf::Tag {
public:
    using swf::Tag::Tag;

    // Override virtual methods
    void read(swf::StreamReader&& stream) override { PYBIND11_OVERRIDE_PURE(void, Tag, read, stream); };
    void write(swf::StreamWriter& stream) override { PYBIND11_OVERRIDE_PURE(void, Tag, write, stream); };
};

constexpr const char* product_id_doc = R"(The product identifier. Known identifiers:
    - 0: Unknown
    - 1: Macromedia Flex for J2EE
    - 2: Macromedia Flex for .NET
    - 3: Adobe Flex)";

constexpr const char* product_edition_doc = R"(The product edition. Known editions:
    - 0: Developer Edition
    - 1: Full Commercial Edition
    - 2: Non Commercial Edition
    - 3: Educational Edition
    - 4: Not For Resale (NFR) Edition
    - 5: Trial Edition
    - 6: None)";

void init_tags(py::module& m) {
    py::class_<swf::Position>(m, "Position", "A coordinate in twips.")
        .def(py::init())
        .def_readwrite("y", &swf::Position::y)
        .def_readwrite("x", &swf::Position::x);

    py::class_<swf::Rect>(
        m, "Rect", "Represent a rectangle region defined by a minimum x and y position and a maximum x and y position.")
        .def(py::init())
        .def("write", &swf::Rect::write, "stream"_a)
        .def("read", &swf::Rect::read, "stream"_a)
        .def("__str__", &swf::Rect::toString)
        .def_readwrite("min", &swf::Rect::min)
        .def_readwrite("max", &swf::Rect::max);

    py::class_<swf::RGB>(m, "RGB", "Represent a 24-bit color with red (r), green (g) and blue (b) values.")
        .def(py::init())
        .def(py::init<uint8_t, uint8_t, uint8_t>(), "r"_a, "g"_a, "b"_a)
        .def("write", &swf::RGB::write, "stream"_a)
        .def("read", &swf::RGB::read, "stream"_a)
        .def("__str__", &swf::RGB::toString)
        .def_readwrite("r", &swf::RGB::r)
        .def_readwrite("g", &swf::RGB::g)
        .def_readwrite("b", &swf::RGB::b);

    py::class_<swf::RGBA, swf::RGB>(m, "RGBA",
        "Represent a  32-bit color with red (r), green (g), blue (b) and alpha (a) values. A value of 255 for the "
        "alpha means it is completly opaque while a value of 0 means it is completly transparent.")
        .def(py::init())
        .def(py::init<uint8_t, uint8_t, uint8_t, uint8_t>(), "r"_a, "g"_a, "b"_a, "a"_a)
        .def_readwrite("a", &swf::RGBA::a);

    py::enum_<swf::TagID>(m, "TagID")
        .value("End", swf::TagID::End)
        .value("SetBackgroundColor", swf::TagID::SetBackgroundColor)
        .value("ProductInfo", swf::TagID::ProductInfo)
        .value("ScriptLimits", swf::TagID::ScriptLimits)
        .value("FileAttributes", swf::TagID::FileAttributes)
        .value("SymbolClass", swf::TagID::SymbolClass)
        .value("Metadata", swf::TagID::Metadata)
        .value("DoABC", swf::TagID::DoABC)
        .value("DefineBinaryData", swf::TagID::DefineBinaryData)
        .value("Unknown", swf::TagID::Unknown);

    py::class_<swf::Tag, PyTag>(m, "Tag", "The base class of any SWF Tag.")
        .def("write", &swf::Tag::write, "Write data to a stream.", "stream"_a)
        .def(
            "read", [](swf::Tag* tag, swf::StreamReader& stream) { tag->read(std::move(stream)); },
            "Read the data from a stream.", "stream"_a)
        .def_property_readonly("tag_name", &swf::Tag::getTagName, "Returns the tag's name")
        .def_property_readonly("tag_id", &swf::Tag::getId, "Returns the tag's id");

    py::class_<swf::UnknownTag, swf::Tag>(m, "UnknownTag", "Represent an unknown tag, not supported by the library.")
        .def(py::init())
        .def_readwrite("id", &swf::UnknownTag::id, "The tag's id");

    py::class_<swf::DefineBinaryDataTag, swf::Tag>(m, "DefineBinaryDataTag",
        "Represent a binary data embeded inside a SWF file. Intended to be used with the SymbolClass tag.")
        .def(py::init())
        .def("getData", &swf::DefineBinaryDataTag::getData, "Let you get the binary data.")
        .def_readwrite("charId", &swf::DefineBinaryDataTag::charId, "The binary's character id");

    py::class_<swf::DoABCTag, swf::Tag>(m, "DoABCTag", "Defines the code to be executed in the AVM2.")
        .def(py::init())
        .def_readwrite("name", &swf::DoABCTag::name, "The name assigned to the bytecode")
        .def_readwrite("is_lazy", &swf::DoABCTag::is_lazy,
            "Indicates if the code should be executed immediatly after being parsed or not")
        .def_readwrite("abcfile", &swf::DoABCTag::abcfile, "Holds the parsed bytecode");

    py::class_<swf::EndTag, swf::Tag>(
        m, "EndTag", "Marks the end of the SWF file. It is also required to end a sprite definition.")
        .def(py::init());

    py::class_<swf::FileAttributesTag, swf::Tag>(m, "FileAttributesTag",
        "If used, it must be the first tag. It defines differents options to configure the Flash Player.")
        .def(py::init())
        .def_property("useNetwork", &swf::FileAttributesTag::useNetwork, &swf::FileAttributesTag::setUseNetwork,
            "Gives network access if the file is run on a local drive")
        .def_property("useGPU", &swf::FileAttributesTag::useGPU, &swf::FileAttributesTag::setUseGPU,
            "Tells the Player to use GPU compositing features if available")
        .def_property("useDirectBlit", &swf::FileAttributesTag::useDirectBlit,
            &swf::FileAttributesTag::setUseDirectBlit,
            "Tells the Flash Player to use hardware acceleration to blit graphics to the screen if available")
        .def_property("useAS3", &swf::FileAttributesTag::useAS3, &swf::FileAttributesTag::setUseAS3,
            "Defines if the SWF contains AVM2 code")
        .def_property("hasMetadata", &swf::FileAttributesTag::hasMetadata, &swf::FileAttributesTag::setHasMetadata,
            "Defines if the SWF has metadata");

    py::class_<swf::MetadataTag, swf::Tag>(m, "MetadataTag",
        "Describe the SWF file to an external process. Used by search engine to access data from the file.")
        .def(py::init())
        .def_readwrite("metadata", &swf::MetadataTag::metadata, "Metadata as XML.");

    py::class_<swf::ProductInfoTag, swf::Tag> product_info(m, "ProductInfoTag",
        "Embed information about the product that were used to compile the SWF file. "
        "http://wahlers.com.br/claus/blog/undocumented-swf-tags-written-by-mxmlc/");
    py::class_<swf::ProductInfoTag::Version>(product_info, "Version")
        .def(py::init())
        .def_readwrite("minor", &swf::ProductInfoTag::Version::minor)
        .def_readwrite("major", &swf::ProductInfoTag::Version::major);

    product_info.def(py::init())
        .def_readwrite("version", &swf::ProductInfoTag::version, "The product version")
        .def_readwrite("productId", &swf::ProductInfoTag::productId, product_id_doc)
        .def_readwrite("edition", &swf::ProductInfoTag::edition, product_edition_doc)
        .def_readwrite("compileDate", &swf::ProductInfoTag::compileDate, "Milliseconds since 1st January 1970")
        .def_readwrite("build", &swf::ProductInfoTag::build, "Extended information about the product's version");

    py::class_<swf::ScriptLimitsTag, swf::Tag>(m, "ScriptLimitsTag", "Settings for the script limits in the AVM2.")
        .def(py::init())
        .def_readwrite("script_timeout_seconds", &swf::ScriptLimitsTag::script_timeout_seconds,
            "Set the maximum number of seconds the player should process ActionScript before asking if the script "
            "should be stopped. The default value varies by platform and is between 15 and 20 seconds.")
        .def_readwrite("max_recursion_depth", &swf::ScriptLimitsTag::max_recursion_depth,
            "Set the max recursion limit. Default is 256");

    py::class_<swf::SetBackgroundColorTag, swf::Tag>(
        m, "SetBackgroundColorTag", "Set the background color of the display.")
        .def(py::init())
        .def_readwrite("color", &swf::SetBackgroundColorTag::color, "Color of the background display");

    py::class_<swf::SymbolClassTag, swf::Tag>(m, "SymbolClassTag",
        "Create an association between symbols in the SWF and ActionScript classes. If the character id is zero, the "
        "class is assiociated with the main timeline. It also means this class is the root class of the SWF.")
        .def(py::init())
        .def("getCharId", &swf::SymbolClassTag::getCharId, "Returns the character id from its class' name.", "name"_a)
        .def_readwrite(
            "symbols", &swf::SymbolClassTag::symbols, "Symbols: <Character id>:<Fully-qualified name of the class>");
}