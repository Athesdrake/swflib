#pragma once
#include "common.h"
#include "info/Class.h"
#include "info/ConstantPool.h"
#include "info/Exception.h"
#include "info/Method.h"
#include "info/Multiname.h"
#include "info/Namespace.h"
#include "info/Script.h"
#include "info/Trait.h"

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

    /* Read method informations */
    virtual void read_methods(StreamReader& stream);
    /* Read method bodies */
    virtual void read_bodies(StreamReader& stream);
    /* Write method informations and return indexes of methods that have a body  */
    virtual std::vector<uint32_t> write_methods(StreamWriter& stream);
    /* Write method bodies */
    virtual void write_bodies(StreamWriter& stream, std::vector<uint32_t> bodies);

    /* Write the abc content to a stream */
    void write(StreamWriter& stream);
    /* Read the abc content from a stream */
    void read(StreamReader& stream);
};
}