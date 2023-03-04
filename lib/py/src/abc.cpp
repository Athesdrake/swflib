#include "abc/AbcFile.hpp"
#include "modules.hpp"
#include <stdexcept>

using namespace swf::abc;

class version {
private:
    AbcFile* abc;

public:
    version(AbcFile* abc) : abc(abc) { }

    uint16_t get_major() { return abc == nullptr ? 0 : abc->major_version; }
    uint16_t get_minor() { return abc == nullptr ? 0 : abc->minor_version; }

    void set_major(uint16_t major) {
        if (abc != nullptr)
            abc->major_version = major;
    }
    void set_minor(uint16_t minor) {
        if (abc != nullptr)
            abc->minor_version = minor;
    }
};

void init_abc(py::module& m, py::class_<AbcFile> abc) {
    py::enum_<MultinameKind>(m, "MultinameKind")
        .value("QName", MultinameKind::QName)
        .value("QNameA", MultinameKind::QNameA)
        .value("RTQName", MultinameKind::RTQName)
        .value("RTQNameA", MultinameKind::RTQNameA)
        .value("RTQNameL", MultinameKind::RTQNameL)
        .value("RTQNameLA", MultinameKind::RTQNameLA)
        .value("Multiname", MultinameKind::Multiname)
        .value("MultinameA", MultinameKind::MultinameA)
        .value("MultinameL", MultinameKind::MultinameL)
        .value("MultinameLA", MultinameKind::MultinameLA)
        .value("Typename", MultinameKind::Typename);

    py::enum_<NamespaceKind>(m, "NamespaceKind")
        .value("STAR", NamespaceKind::STAR)
        .value("Namespace", NamespaceKind::Namespace)
        .value("Package", NamespaceKind::Package)
        .value("PackageInternal", NamespaceKind::PackageInternal)
        .value("Protected", NamespaceKind::Protected)
        .value("Explicit", NamespaceKind::Explicit)
        .value("StaticProtected", NamespaceKind::StaticProtected)
        .value("Private", NamespaceKind::Private);

    py::enum_<TraitKind>(m, "TraitKind")
        .value("Slot", TraitKind::Slot)
        .value("Method", TraitKind::Method)
        .value("Getter", TraitKind::Getter)
        .value("Setter", TraitKind::Setter)
        .value("Class", TraitKind::Class)
        .value("Function", TraitKind::Function)
        .value("Const", TraitKind::Const);

    py::enum_<TraitAttr>(m, "TraitAttr")
        .value("Final", TraitAttr::Final)
        .value("Override", TraitAttr::Override)
        .value("Metadata", TraitAttr::Metadata);

    py::class_<Trait> trait(m, "Trait");

    py::class_<Trait::Slot>(trait, "Slot")
        .def(py::init())
        .def_readwrite("type", &Trait::Slot::type)
        .def_readwrite("kind", &Trait::Slot::kind);

    trait.def(py::init())
        .def(py::init<AbcFile*>())
        .def("write", &Trait::write, "stream"_a)
        .def("read", &Trait::read, "stream"_a)
        .def("__repr__", &Trait::toString)
        .def_readwrite("slot", &Trait::slot)
        .def_readwrite("slot_id", &Trait::slot_id)
        .def_readwrite("kind", &Trait::kind)
        .def_readwrite("index", &Trait::index)
        .def_readwrite("attr", &Trait::attr)
        .def_property("name", &Trait::get_name, &Trait::rename);

    py::class_<version>(m, "version")
        .def_property("minor", &version::get_minor, &version::set_minor)
        .def_property("major", &version::get_major, &version::set_major);

    py::class_<Option>(m, "Option")
        .def(py::init())
        .def_readwrite("value", &Option::value)
        .def_readwrite("kind", &Option::kind);

    py::class_<Exception>(m, "AbcException")
        .def(py::init())
        .def(py::init<AbcFile*>(), "abc"_a)
        .def("write", &Exception::write, "stream"_a)
        .def("read", &Exception::read, "stream"_a)
        .def_readwrite("type", &Exception::type)
        .def_readwrite("to", &Exception::to)
        .def_readwrite("target", &Exception::target)
        .def_readwrite("from_", &Exception::from)
        .def_property(
            "var_name", &Exception::get_var_name, py::overload_cast<std::string&>(&Exception::rename_var_name));

    py::class_<Method>(m, "Method")
        .def(py::init())
        .def_readwrite("traits", &Method::traits)
        .def_readwrite("return_type", &Method::return_type)
        .def_readwrite("params", &Method::params)
        .def_readwrite("param_names", &Method::param_names)
        .def_readwrite("optional", &Method::optional)
        .def_readwrite("name", &Method::name)
        .def_readwrite("max_stack", &Method::max_stack)
        .def_readwrite("max_scope_depth", &Method::max_scope_depth)
        .def_readwrite("local_count", &Method::local_count)
        .def_readwrite("init_scope_depth", &Method::init_scope_depth)
        .def_readwrite("flags", &Method::flags)
        .def_readwrite("exceptions", &Method::exceptions)
        .def_readwrite("code", &Method::code);

    py::class_<Class>(m, "Class")
        .def(py::init())
        .def("write", &Class::write, "stream"_a)
        .def("read", &Class::read, "stream"_a)
        .def_readwrite("protected_ns", &Class::protected_ns)
        .def_readwrite("itraits", &Class::itraits)
        .def_readwrite("interfaces", &Class::interfaces)
        .def_readwrite("iinit", &Class::iinit)
        .def_readwrite("flags", &Class::flags)
        .def_readwrite("ctraits", &Class::ctraits)
        .def_readwrite("cinit", &Class::cinit)
        .def_property("super_name", &Class::get_super_name, &Class::rename_super)
        .def_property("name", &Class::get_name, &Class::rename)
        .def_property("isSealed", &Class::isSealed, &Class::setSealed)
        .def_property("isProtected", &Class::isProtected, &Class::setProtected)
        .def_property("isInterface", &Class::isInterface, &Class::setInterface)
        .def_property("isFinal", &Class::isFinal, &Class::setFinal);

    py::class_<Script>(m, "Script")
        .def(py::init())
        .def("write", &Script::write, "stream"_a)
        .def("read", &Script::read, "stream"_a)
        .def_readwrite("init", &Script::init)
        .def_readwrite("traits", &Script::traits);

    py::class_<Namespace>(m, "Namespace")
        .def(py::init())
        .def("write", &Namespace::write, "stream"_a)
        .def("read", &Namespace::read, "stream"_a)
        .def_readwrite("name", &Namespace::name)
        .def_readwrite("kind", &Namespace::kind);

    py::class_<Multiname::MultinameData::QName>(m, "MD_QName")
        .def_readwrite("ns", &Multiname::MultinameData::QName::ns)
        .def_readwrite("name", &Multiname::MultinameData::QName::name);
    py::class_<Multiname::MultinameData::RTQName>(m, "MD_RTQName")
        .def_readwrite("name", &Multiname::MultinameData::RTQName::name);
    py::class_<Multiname::MultinameData::Multiname>(m, "MD_Multiname")
        .def_readwrite("name", &Multiname::MultinameData::Multiname::name)
        .def_readwrite("ns_set", &Multiname::MultinameData::Multiname::ns_set);
    py::class_<Multiname::MultinameData::MultinameL>(m, "MD_MultinameL")
        .def_readwrite("ns_set", &Multiname::MultinameData::MultinameL::ns_set);
    py::class_<Multiname::MultinameData::TypeName>(m, "MD_TypeName")
        .def_readwrite("qname", &Multiname::MultinameData::TypeName::qname);
    py::class_<Multiname::MultinameData>(m, "MultinameData")
        .def_readwrite("qname", &Multiname::MultinameData::qname)
        .def_readwrite("rtqname", &Multiname::MultinameData::rtqname)
        .def_readwrite("multiname", &Multiname::MultinameData::multiname)
        .def_readwrite("multiname_l", &Multiname::MultinameData::multiname_l)
        .def_readwrite("type_name", &Multiname::MultinameData::type_name);
    py::class_<Multiname>(m, "Multiname")
        .def(py::init<>())
        .def("write", &Multiname::write, "stream"_a)
        .def("read", &Multiname::read, "stream"_a)
        .def_property("name_index", &Multiname::get_name_index, &Multiname::set_name_index)
        .def_readwrite("kind", &Multiname::kind)
        .def_readwrite("data", &Multiname::data)
        .def_readwrite("types", &Multiname::types);

    py::class_<ConstantPool>(m, "ConstantPool")
        .def(py::init())
        .def("write", &ConstantPool::write, "stream"_a)
        .def("read", &ConstantPool::read, "stream"_a)
        .def_readwrite("uintegers", &ConstantPool::uintegers)
        .def_readwrite("strings", &ConstantPool::strings)
        .def_readwrite("ns_sets", &ConstantPool::ns_sets)
        .def_readwrite("namespaces", &ConstantPool::namespaces)
        .def_readwrite("multinames", &ConstantPool::multinames)
        .def_readwrite("integers", &ConstantPool::integers)
        .def_readwrite("doubles", &ConstantPool::doubles);

    abc.def(py::init())
        .def("write", &AbcFile::write, "Write the abc content to a stream", "stream"_a)
        .def("read", &AbcFile::read, "Read the abc content from a stream", "stream"_a)
        .def_readwrite("scripts", &AbcFile::scripts, "list of all abc's scripts")
        .def_readwrite("methods", &AbcFile::methods, "list of all abc's methods")
        .def_readwrite("cpool", &AbcFile::cpool, "abc constant pool, contains all constant values")
        .def_readwrite("classes", &AbcFile::classes, "list of all abc's classes")
        .def_property(
            "version", [](AbcFile& abc) { return version(&abc); },
            [](AbcFile& abc, py::tuple v) {
                abc.major_version = v[0].cast<uint16_t>();
                abc.minor_version = v[1].cast<uint16_t>();
            },
            "abc's version");
}