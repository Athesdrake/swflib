#pragma once
#include "swflib.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

namespace py  = pybind11;
namespace abc = swf::abc;
using namespace pybind11::literals;

/* Mark these types to be opaques. https://pybind11.readthedocs.io/en/stable/advanced/cast/stl.html#making-opaque-types
   It means it won't be converted into a list in Python but it will have its own class.
   It reduces copies from python to c++ and let you use `list.append` and other methods.
   In short, it makes the lists much faster.
*/
PYBIND11_MAKE_OPAQUE(std::unordered_map<std::string, swf::DoABCTag*>);
PYBIND11_MAKE_OPAQUE(std::unordered_map<uint16_t, std::string>);
PYBIND11_MAKE_OPAQUE(std::vector<abc::Class>);
PYBIND11_MAKE_OPAQUE(std::vector<abc::Exception>);
PYBIND11_MAKE_OPAQUE(std::vector<abc::Method>);
PYBIND11_MAKE_OPAQUE(std::vector<abc::Multiname>);
PYBIND11_MAKE_OPAQUE(std::vector<abc::Namespace>);
PYBIND11_MAKE_OPAQUE(std::vector<abc::Option>);
PYBIND11_MAKE_OPAQUE(std::vector<abc::Script>);
PYBIND11_MAKE_OPAQUE(std::vector<abc::Trait>);
PYBIND11_MAKE_OPAQUE(std::vector<double>);
PYBIND11_MAKE_OPAQUE(std::vector<int32_t>);
PYBIND11_MAKE_OPAQUE(std::vector<std::shared_ptr<swf::Tag>>);
PYBIND11_MAKE_OPAQUE(std::vector<std::string>);
PYBIND11_MAKE_OPAQUE(std::vector<std::vector<uint32_t>>);
PYBIND11_MAKE_OPAQUE(std::vector<swf::DefineBinaryDataTag*>);
PYBIND11_MAKE_OPAQUE(std::vector<uint32_t>);
PYBIND11_MAKE_OPAQUE(std::vector<uint8_t>);

void init_swf(py::module& m);
void init_abc(py::module& m, py::class_<swf::abc::AbcFile> abc);
void init_stream(py::module& m);
void init_tags(py::module& m);
