# CastleNBT

A C++23 library for creating and serializing Minecraft NBT
(Named Binary Tag) data.

CastleNBT is primarily a learning project created while developing
**CastleCraft**, my own Minecraft server implementation.

> [!NOTE]
> **Current version: `0.1.0`**
>
> CastleNBT is in early development. Version `0.1.0` focuses on creating
> NBT structures and serializing them to the binary NBT format.
>
> Reading/parsing NBT is planned for a future release.

## Contents

- [Features](#features)
- [Supported Tags](#supported-tags)
- [Requirements](#requirements)
- [Installation](#installation)
  - [CMake FetchContent](#cmake-fetchcontent)
  - [Git Submodule](#git-submodule)
- [Quick Start](#quick-start)
- [Usage](#usage)
  - [Creating a Compound](#creating-a-compound)
  - [Nested Compounds](#nested-compounds)
  - [Lists](#lists)
  - [Arrays](#arrays)
  - [Serializing NBT](#serializing-nbt)
- [API Overview](#api-overview)
- [Building CastleNBT](#building-castlenbt)
- [Running Tests](#running-tests)
- [Version 0.1.0](#version-010)
- [Roadmap](#roadmap)
- [Project Structure](#project-structure)
- [Motivation](#motivation)
- [License](#license)

## Features

- C++23
- All standard NBT tag types
- Nested compound tags
- Homogeneous NBT lists
- Byte, Int and Long arrays
- Binary NBT serialization
- Big-endian numeric encoding
- Serialization to `std::vector<std::uint8_t>`
- CMake support
- CMake `FetchContent` support
- GoogleTest test suite

## Supported Tags

| NBT Tag | Support |
| --- | --- |
| `TAG_End` | Internal |
| `TAG_Byte` | ✅ |
| `TAG_Short` | ✅ |
| `TAG_Int` | ✅ |
| `TAG_Long` | ✅ |
| `TAG_Float` | ✅ |
| `TAG_Double` | ✅ |
| `TAG_Byte_Array` | ✅ |
| `TAG_String` | ✅ |
| `TAG_List` | ✅ |
| `TAG_Compound` | ✅ |
| `TAG_Int_Array` | ✅ |
| `TAG_Long_Array` | ✅ |

> In version `0.1.0`, support refers to creating and serializing these
> tags. Binary NBT parsing is not implemented yet.

## Requirements

- C++23 compatible compiler
- CMake 3.24 or newer

GoogleTest is required only when building the CastleNBT test suite.

## Installation

### CMake FetchContent

The recommended way to use CastleNBT in another CMake project is with
`FetchContent`.

```cmake
include(FetchContent)

FetchContent_Declare(
    castleNBT
    GIT_REPOSITORY https://github.com/kronborg6/CastleNBT.git
    GIT_TAG v0.1.0
)

FetchContent_MakeAvailable(castleNBT)

target_link_libraries(your_target
    PRIVATE castleNBT
)
```

CastleNBT can then be included with:

```cpp
#include <castle/nbt.hpp>
```

Using a version tag such as `v0.1.0` instead of the `main` branch is
recommended because it keeps your builds reproducible.

### Git Submodule

CastleNBT can also be included as a Git submodule.

```bash
git submodule add https://github.com/kronborg6/CastleNBT.git external/castleNBT
git submodule update --init --recursive
```

Then add CastleNBT to your `CMakeLists.txt`:

```cmake
add_subdirectory(external/castleNBT)

target_link_libraries(your_target
    PRIVATE castleNBT
)
```

## Quick Start

```cpp
#include <castle/nbt.hpp>

using castle::nbt::Nbt;

int main() {
    auto root = Nbt::makeCompound("");

    root.add(Nbt::makeString("name", "Steve"));
    root.add(Nbt::makeInt("health", 20));
    root.add(Nbt::makeFloat("speed", 0.1f));

    auto bytes = root.compile();
}
```

`compile()` serializes the NBT structure and returns the resulting
binary data as a `std::vector<std::uint8_t>`.

## Usage

### Creating a Compound

Create a compound with `Nbt::makeCompound()` and add named tags using
`add()`.

```cpp
#include <castle/nbt.hpp>

using castle::nbt::Nbt;

auto root = Nbt::makeCompound("");

root.add(Nbt::makeString("name", "Kronborg"));
root.add(Nbt::makeInt("health", 20));
root.add(Nbt::makeLong("xp", 12345));
root.add(Nbt::makeFloat("speed", 0.1f));
```

This represents a structure similar to:

```text
{
    name: "Kronborg",
    health: 20,
    xp: 12345L,
    speed: 0.1f
}
```

### Nested Compounds

Compounds can contain other compounds.

```cpp
auto root = Nbt::makeCompound("");

auto player = Nbt::makeCompound("player");

player.add(Nbt::makeString("name", "Steve"));
player.add(Nbt::makeInt("health", 20));

auto position = Nbt::makeCompound("position");

position.add(Nbt::makeDouble("x", 10.5));
position.add(Nbt::makeDouble("y", 64.0));
position.add(Nbt::makeDouble("z", -32.5));

player.add(std::move(position));
root.add(std::move(player));

auto bytes = root.compile();
```

The resulting structure is similar to:

```text
{
    player: {
        name: "Steve",
        health: 20,
        position: {
            x: 10.5,
            y: 64.0,
            z: -32.5
        }
    }
}
```

### Lists

NBT lists contain unnamed elements and all elements must use the same
NBT tag type.

```cpp
auto numbers = Nbt::makeList("numbers");

numbers.append(std::int32_t{10});
numbers.append(std::int32_t{20});
numbers.append(std::int32_t{30});
```

Lists can also contain more complex NBT types such as compounds:

```cpp
auto inventory = Nbt::makeList("inventory");

auto sword = Nbt::makeCompound("");
sword.add(Nbt::makeString("id", "minecraft:diamond_sword"));
sword.add(Nbt::makeInt("count", 1));

auto pickaxe = Nbt::makeCompound("");
pickaxe.add(Nbt::makeString("id", "minecraft:diamond_pickaxe"));
pickaxe.add(Nbt::makeInt("count", 1));

inventory.append(std::move(sword));
inventory.append(std::move(pickaxe));

auto root = Nbt::makeCompound("");
root.add(std::move(inventory));

auto bytes = root.compile();
```

CastleNBT checks that elements added to an NBT list use the same tag
type.

For example, adding an `TAG_Int` to a list containing `TAG_String`
elements will result in an error.

### Arrays

NBT defines three specialized array types:

- `TAG_Byte_Array`
- `TAG_Int_Array`
- `TAG_Long_Array`

CastleNBT supports all three:

```cpp
auto root = Nbt::makeCompound("");

root.add(
    Nbt::makeByteArray(
        "bytes",
        {1, 2, 3}
    )
);

root.add(
    Nbt::makeIntArray(
        "ints",
        {100, 200, 300}
    )
);

root.add(
    Nbt::makeLongArray(
        "longs",
        {1000, 2000, 3000}
    )
);

auto bytes = root.compile();
```

### Serializing NBT

There are two ways to serialize an NBT structure.

#### Return a new byte vector

```cpp
std::vector<std::uint8_t> data = root.compile();
```

#### Append to an existing byte vector

```cpp
std::vector<std::uint8_t> data;

root.compile(data);
```

The resulting byte vector contains the binary NBT representation of
the structure.

## API Overview

The main CastleNBT type is:

```cpp
castle::nbt::Nbt
```

### Creating tags

Tags can be created using the static factory functions:

```cpp
Nbt::makeByte(...)
Nbt::makeShort(...)
Nbt::makeInt(...)
Nbt::makeLong(...)
Nbt::makeFloat(...)
Nbt::makeDouble(...)
Nbt::makeByteArray(...)
Nbt::makeString(...)
Nbt::makeList(...)
Nbt::makeCompound(...)
Nbt::makeIntArray(...)
Nbt::makeLongArray(...)
```

### Compounds

Add a child to a compound with:

```cpp
compound.add(child);
```

### Lists

Append an element to a list with:

```cpp
list.append(value);
```

### Access tag information

The tag type can be retrieved with:

```cpp
auto type = nbt.tag();
```

The tag name can be retrieved with:

```cpp
auto name = nbt.name();
```

The underlying value can be retrieved with:

```cpp
const auto& value = nbt.value();
```

### Rename a tag

```cpp
nbt.rename("newName");
```

or remove its name:

```cpp
nbt.clearName();
```

### Serialization

Serialize an NBT value with:

```cpp
auto data = nbt.compile();
```

or:

```cpp
std::vector<std::uint8_t> data;
nbt.compile(data);
```

## Building CastleNBT

Clone the repository:

```bash
git clone https://github.com/kronborg6/CastleNBT.git
cd CastleNBT
```

Configure the project:

```bash
cmake -S . -B build
```

Build it:

```bash
cmake --build build
```

When CastleNBT is built as the top-level project, the development tool
is also built:

```text
castleNBTTool
```

## Running Tests

CastleNBT uses GoogleTest for automated testing.

Configure and build the project:

```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build
```

Run the tests with:

```bash
ctest --test-dir build --output-on-failure
```

The serialization tests compare generated NBT byte streams against
expected binary output.

## Version 0.1.0

`0.1.0` is the first development release of CastleNBT.

The primary goal of this version is to provide the core NBT data model
and binary serialization.

### Implemented

- NBT value representation
- All standard NBT value types
- Compound tags
- List tags
- Homogeneous list validation
- Nested NBT structures
- Byte arrays
- Int arrays
- Long arrays
- Binary NBT serialization
- Big-endian numeric encoding
- Automated serialization tests
- CMake integration

The API should still be considered unstable during the `0.x` releases
and may change as the library develops.

## Roadmap

Planned features include:

- Binary NBT parsing
- Reading NBT from files
- Writing NBT directly to files
- NBT to SNBT conversion
- SNBT parsing
- Human-readable NBT output
- Parsing human-readable NBT
- Compressed NBT support

See [TODO.md](TODO.md) for the current development notes.

## Project Structure

```text
CastleNBT/
├── include/
│   └── castle/          Public library headers
│
├── src/                 Library implementation
│
├── tests/               GoogleTest test suite
│
├── tools/               Development/testing executable
│
├── CMakeLists.txt
├── LICENSE
├── README.md
└── TODO.md
```

## Motivation

CastleNBT was created as part of **CastleCraft**, a Minecraft server
project that I am developing from scratch.

Instead of using an existing NBT library, I wanted to implement the
format myself to better understand topics such as:

- binary serialization
- byte ordering and endianness
- binary data formats
- recursive data structures
- type-safe C++ APIs
- memory and data representation
- the Minecraft protocol and file formats

The project is also an opportunity for me to improve my understanding
of modern C++, library design, CMake and automated testing.

## License

CastleNBT is licensed under the [MIT License](LICENSE).
