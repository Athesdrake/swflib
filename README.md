# swflib
swflib is a SWF parsing library in C++ with bindings in python.

Note:
This repository contains only the C++ code. To see the python code go check [pyswflib](https://github.com/Athesdrake/pyswflib).

## Build from source
First thing first, download the source code or clone the repository:
```sh
git clone https://github.com/Athesdrake/swflib
cd swflib
```

### Linux users
Then, make sure you have a C++17 compiler and CMake installed.
To install it:
 - apt: `apt install build-essential cmake`
 - pacman: `pacman -Sy base-devel cmake`
 - dnf: `dnf install @development-tools cmake`

#### Dependencies
##### Without vcpkg
In order to compile `swflib`, you will need `zlib`, `liblzma` and `pybind11` (optional).
You can install zlib and liblzma headers using your package manager. It depends on your distribution so you have to search yourself.
Read the instructions to install [pybind11](https://pybind11.readthedocs.io/en/stable/installing.html).

##### Using vcpkg
[vcpkg](https://vcpkg.io/en/index.html) might be easier to install dependencies.
First [install vcpkg](https://vcpkg.io/en/getting-started.html), then install dependencies using `vcpkg install zlib liblzma pybind11`.

### Windows users
You need to install [Visual Studio](https://visualstudio.microsoft.com/downloads/) (community edition is fine).
Visual Studio comes with a bundled CMake version. If it's not the case, install CMake from [here](https://cmake.org/download/).

#### Dependencies
In order to compile `swflib`, you will need `zlib`, `liblzma` and `pybind11` (optional).
You can easily install these dependencies using [vcpkg](https://vcpkg.io/en/index.html).

To install vcpkg, read instructions from the [documentation](https://vcpkg.io/en/getting-started.html).

Once vcpkg is installed, install the dependencies: `vcpkg install zlib liblzma pybind11`.

If you want to compile to x64, you might need to install x64 triplets like so: `vcpkg install zlib:x64-windows liblzma:x64-windows pybind11:x64-windows`.

### Compiling
You can either open the folder with an IDE supporting CMake or build it from your terminal.

If you are using CMake from your terminal, don't forget to `vcpkg integrate install`. Note the output of this command and replace `[path to vcpkg]/scripts/buildsystems/vcpkg.cmake` by the command's output.
```sh
mkdir build
cd build
cmake .. "-DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake"
```

Afterward, you'll be able to build swflib:
```sh
cmake --build . # Build everything (you can also use '--target all')
cmake --build . --target swflib # Build only the library
cmake --build . --target pyswflib # Build only the python bindings
```

To run tests:
`ctest -T test`
