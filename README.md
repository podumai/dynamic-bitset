# DYNAMIC-BITSET

![Ubuntu](https://github.com/podumai/dynamic-bitset/actions/workflows/ubuntu-latest.yaml/badge.svg)
![Windows](https://github.com/podumai/dynamic-bitset/actions/workflows/windows-latest.yaml/badge.svg)
![MacOs](https://github.com/podumai/dynamic-bitset/actions/workflows/macos-latest.yaml/badge.svg)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Table of Contents

1. [Description](#description);
2. [Build](#build):
   - [CMakePresets build](#cmakepresets-build);
   - [CMakeWorkflows build](#cmakeworkflows-build);
   - [Regular build](#regular-build).
3. [Tests](#tests);
4. [Benchmarks](#benchmarks).

## Description

The **DynamicBitset** class represents a set of bits. The size of the **DynamicBitset** can be set at runtime via constructor or `Resize` method.  
It supports bitwise operations with behaviour equivalent to builtin unsigned integral types. The **DynamicBitset** class provides ability to manipulate with bits via overloaded `operator[]` and `At`, `Test` member functions. It also supports iterators with proxy class for bit manipulation.
The main problem that **DynamicBitset** class is trying to solve is to represent a subset of finite set. For example, you can use it to represent the visited nodes in graph using BFS, DFS.  
The **DynamicBitset** class designed to provide fast bit manipulations and space-efficient memory storage.

## Build

> [!IMPORTANT]  
> To build tests, documentation or benchmarks you need to have:  
>
> - C/C++ compiler (C++20 support);
> - CMake (v3.23.0);
> - google test package (for tests);
> - google benchmark package (for benchmarks);
> - doxygen (for documentation).

The building process is configured with three options.  

| Option | Supported values | Default value |
| :---: | :---: | :---: |
| BUILD_TESTS | ON/OFF | ON |
| BUILD_BENCHMARKS | ON/OFF | OFF |
| GENERATE_DOCS | ON/OFF | OFF |

> [!IMPORTANT]  
> The following statements are correct:  
>
> - If you are missing google test and you specify option then it will be installed on project level;
> - If you are missing google benchmark and you specify option then it will be installed on project level;
> - If you specify doxygen option and it is locally installed then generated documentation will be located in `docs` directory.

### CMakePresets build

> [!WARNING]  
> Building the project with presets requires `CMake` version greater than or equal to **3.25**.  

The project offers the build with `CMakePresets` support for configuration and building process.  
Supported presets can be listed with commands:

```shell
cmake --list-presets=configure
cmake --list-presets=build
```  

With your chosen configuration and build preset finally execute this commands:  

```shell
cmake --preset <preset-type> [-D <project-options>]
cmake --build --preset <preset-type>
```

All build artifacts and executables will be in `build/<preset-type>` folder.

### CMakeWorkflows build

> [!WARNING]  
> Building the project with workflows requires `CMake` version greater than or equal to **3.25**.  

On newer `CMake` you can just use the single command to configure and build the project.  
First of all, check the supported workflows with: `cmake --list-presets=workflow`  
Choose your appropriate workflow and execute this command: `cmake --workflow --preset <workflow-preset>`  
> [!WARNING]  
> Workflows do not accept any configuration parameters and you can not pass `-D<name>:<type>=<value>`.  
> If you are going to build the benchmarks or provide additional configuration then use [CMakePresets build](#cmakepresets-build) or [Regular build](#regular-build).

### Regular build

If your `CMake` does not support workflows or presets then you can build the project as usual.  
Execute commands below with your options (if any will be specified, e.g. `-DCMAKE_LINKER_TYPE=<linker-type>`):

```shell
cmake [-D <project-options>] -DCMAKE_BUILD_TYPE=Release -S . -B ./build
cmake --build ./build
```

## Tests

> [!CAUTION]  
> This class was optimized and tested for machines with Little-Endian byte ordering.

Tests are written using gtest framework and located in `<build-dir>/tests/bin` folder with executable name `tests`.  
Run the tests with `ctest` to see if all working correctly: `ctest --test-dir <build-dir>/tests`.

## Benchmarks

Benchmarks are written using google benchmark framework and located in `<build-dir>/benchmark/bin` directory.  
All possible benchmarks to built locates in subdirectories:  

- bits (bits::DynamicBitset);
- boost (boost::dynamic_bitset);
- std (std::vector\<bool>).

You can run all the benchmarks available for each container by running the executable in `<build-dir>/benchmark/bin/<namespace>`.  
Also you can provide regular expression to filter the benchmarks as with usual google benchmark executable.  
To see the certain benchmarks run: `./<build-dir>/benchmark/bin/<container>/benchmark --benchmark_filter=<regex>`.  
> [!TIP]  
> Benchmarks can be filtered by <ins>testing operations</ins>, <ins>container names</ins> or even by <ins>block types</ins>.  
> Filtering benchmarks by block types only available for ***bits::DynamicBitset*** and ***boost::dynamic_bitset***.  
> General benchmark name: `[(const )<container>::<operation>]`.  
> General benchmark name example: `[const bits::DynamicBitset<unsigned>::PushBack()]`.  
> Example use: `--benchmark_filter='(<unsigned ?(char|short|long( long)?)?>)?::(push_back|PushBack){1}\(\)'`.

<details>
  <summary>List of available benchmark names</summary>
  
### Benchmark names
  
  | Benchmark | Name (operation) |
  | :---: | --- |
  | Default constructor | vector()<br>dynamic_bitset()<br>DynamicBitset() |
  | Copy constructor | vector(const vector&)<br>dynamic_bitset(const dynamic_bitset&)<br>DynamicBitset(const DynamicBitset&) |
  | Move constructor | vector(vector&&)<br>dynamic_bitset(dynamic_bitset&&)<br>DynamicBitset(DynamicBitset&&) |
  | Copy assignment operator | operator=(const vector&)<br>operator=(const dynamic_bitset&)<br>operator=(const DynamicBitset&) |
  | Move assignment operator | operator=(vector&&)<br>operator=(dynamic_bitset&&)<br>operator=(DynamicBitset&&) |
  | Push back | push_back() (vector/dynamic_bitset)<br>PushBack() (DynamicBitset) |
  | Pop back | pop_back() (vector/dynamic_bitset)<br>PopBack() (DynamicBitset) |
  | Subscript operator | operator[] |
  | At method | at() (vector/dynamic_bitset)<br>At() (DynamicBitset)|
  | Test method | test() (dynamic_bitset)<br>Test() (DynamicBitset) |
  | Set method | set() (dynamic_bitset)<br>Set() (DynamicBitset) |
  | Reset method | reset() (dynamic_bitset)<br>Reset() (DynamicBitset) |
  | Flip method | flip() (vector/dynamic_bitset)<br>Flip() (DynamicBitset) |
  | Swap method | swap() (vector/dynamic_bitset)<br>Swap() (DynamicBitset) |
  | All method | all() (dynamic_bitset)<br>All() (DynamicBitset) |
  | Any method | any() (dynamic_bitset)<br>Any() (DynamicBitset) |
  | None method | none() (dynamic_bitset)<br>None() (DynamicBitset) |
  | Front method | front() (vector)<br>Front() (DynamicBitset) |
  | Back method | back() (vector)<br>Back() (DynamicBitset) |
  | Count method | Count() (DynamicBitset) |
  | Empty method | empty() (vector/dynamic_bitset)<br>Empty() (DynamicBitset) |
  | Size method | size() (vector/dynamic_bitset)<br>Size() (DynamicBitset) |
  | Capacity method | capacity() (vector/dynamic_bitset)<br>Capacity() (DynamicBitset) |

</details>
