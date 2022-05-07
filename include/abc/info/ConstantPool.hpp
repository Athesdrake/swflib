#pragma once
#include "Multiname.hpp"
#include "Namespace.hpp"
#include "abc/common.hpp"

namespace swf::abc {
class ConstantPool {
public:
    std::vector<int32_t> integers;
    std::vector<uint32_t> uintegers;
    std::vector<double> doubles;
    std::vector<std::string> strings;
    std::vector<Namespace> namespaces;
    std::vector<std::vector<uint32_t>> ns_sets;
    std::vector<Multiname> multinames;

    ConstantPool();

    void read(StreamReader& stream);
    void write(StreamWriter& stream);

private:
    AbcFile* abc;
};
}