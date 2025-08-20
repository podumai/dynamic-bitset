# DYNAMIC-BITSET

## Table of Contents

1. [Description](#description);
2. [Build](#build):
   - [CMakePresets build](#cmakepresets-build);
   - [Regular build](#regular-build).
3. [Tests](#tests);
4. [Benchmarks](#benchmarks).

## Description

The **DynamicBitset** class represents a set of bits. The size of the **DynamicBitset** can be set at runtime via constructor or `Resize` method.  
It supports bitwise operations with behaviour equivalent to builtin unsigned integral types. The **DynamicBitset** class provides ability  
to manipulate with bits via overloaded `operator[]` and `At`, `Test` member functions. It also supports iterators with proxy class for bit manipulation.  
The main problem that **DynamicBitset** class is trying to solve is to represent a subset of finite set. For example you can use it  
to represent the visited nodes in graph using BFS, DFS.  
The **DynamicBitset** class designed to provide fast bit manipulations and space-efficient memory storage.

## Build

> [!IMPORTANT]  
> To build tests and/or benchmarks you need to have:  
> * C/C++ compiler;
> * CMake;
> * google test package;
> * google benchmark package.

The building process is configured with two options.   
| Option | Supported values | Default value |
| :---: | :---: | :---: |
| BUILD_TESTS | ON/OFF | ON |
| BUILD_BENCHMARKS | ON/OFF | OFF |  

### CMakePresets build

The project offers the build with **CMakePresets** support for configuration and building process.  
Supported presets can be listed with commands:
```
cmake --list-presets=configure
cmkae --list-presets=build
```  
With your chosen configuration and build preset finally execute this commands:  
```
cmake --preset <preset-type> [-D <project-options>]
cmake --build --preset <preset-type>
```
All build artifacts and executables will be in `build/<preset-type>` folder.

### Regular build

Also you can build the project without predefined presets with:
```
cmake [-D <project-options>] -DCMAKE_BUILD_TYPE=Release -S . -B ./build
cmake --build ./build
```

## Tests

Tests are written using gtest framework and located in `<build-dir>/test/bin` folder with executable name `test`.  
Run the tests with ctest to see if all working properly: `ctest --test-dir <build-dir>/test`.

## Benchmarks

Benchmarks are written using google benchmark framework and located in `<build-dir>/benchmark/bin` directory.  
All possible benchmarks to built locates in subdirectories:  
* bits (bits::DynamicBitset);
* boost (boost::dynamic_bitset);
* std (std::vector<bool>).

You can run all the benchmarks available for each container by running the executable in `<build-dir>/benchmark/bin/<container>`.  
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
  
  ### Benchmark names:
  
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
  | Count method | count() (dynamic_bitset)<br>Count() (DynamicBitset) |
  | Empty method | empty() (vector/dynamic_bitset)<br>Empty() (DynamicBitset) |
  | Size method | size() (vector/dynamic_bitset)<br>Size() (DynamicBitset) |
  | Capacity method | capacity() (vector/dynamic_bitset)<br>Capacity() (DynamicBitset) |
</details>
