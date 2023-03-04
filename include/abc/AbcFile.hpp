#pragma once
#include "common.hpp"
#include "info/Class.hpp"
#include "info/ConstantPool.hpp"
#include "info/Exception.hpp"
#include "info/Method.hpp"
#include "info/Multiname.hpp"
#include "info/Namespace.hpp"
#include "info/Script.hpp"
#include "info/Trait.hpp"

namespace swf::abc {
/* Represent an ABC file which contains compiled programs: constant data, instructions and various kinds of metdata
 */
class AbcFile {
public:
    uint16_t minor_version; // abc's minor version
    uint16_t major_version; // abc's major version

    ConstantPool cpool; // abc constant pool, contains all constant values

    std::vector<Method> methods; // list of all abc's methods
    std::vector<Class> classes; // list of all abc's classes
    std::vector<Script> scripts; // list of all abc's scripts

    AbcFile();
    AbcFile(StreamReader& stream);
    AbcFile(StreamReader&& stream);

    /* Write the abc content to a stream */
    void write(StreamWriter& stream);
    /* Read the abc content from a stream */
    void read(StreamReader& stream);

    /* Get the name of QName as a ref from the multiname index */
    std::string& qname(uint32_t& index);
    /* Get the name of QName as a ref */
    std::string& qname(Multiname& mn);
    /* Get a multiname's name from its index */
    std::string str(uint32_t& index);
    /* Get a multiname's name */
    std::string str(Multiname& mn);
    /* Get a namespace set's name */
    std::string str(std::vector<uint32_t>& ns_set);
    /* Get a namespace's name */
    std::string& str(Namespace& ns);
    /* Get a multiname's namespace's name */
    std::string ns(Multiname& mn);
    /* Get the fully qualified name of a class: package::ClassName */
    std::string fqn(Class& klass);

    /* Read method informations */
    virtual void read_methods(StreamReader& stream);
    /* Read method bodies */
    virtual void read_bodies(StreamReader& stream);
    /* Write method informations and return indexes of methods that have a body  */
    virtual std::vector<uint32_t> write_methods(StreamWriter& stream);
    /* Write method bodies */
    virtual void write_bodies(StreamWriter& stream, std::vector<uint32_t> bodies);
};
}