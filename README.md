# CastleNBT

A C++23 library for creating, reading, and writing Minecraft NBT (Named Binary Tag) data.

CastleNBT provides a high-level API for working with NBT structures and a low-level API for reading and writing individual NBT tags.

The library was created as part of [CastleCraft](https://github.com/kronborg6), a Minecraft server project written from scratch in C++.

> [!WARNING]
> **Development branch — Upcoming version 0.2.0**
>
> CastleNBT is still in early development. The `dev` branch contains significant breaking changes compared to version `0.1.0`.
>
> The new reading API and some low-level functionality are still under development. APIs may change before the 0.2.0 release.

## Contents

- [Features](#features)
- [Supported NBT Tags](#supported-nbt-tags)
- [Requirements](#requirements)
- [Installation](#installation)
- [Quick Start](#quick-start)
- [Usage](#usage)
  - [Creating NBT Tags](#creating-nbt-tags)
  - [Compounds](#compounds)
  - [Lists](#lists)
  - [Arrays](#arrays)
  - [Accessing NBT Values](#accessing-nbt-values)
  - [Writing NBT](#writing-nbt)
  - [Reading NBT](#reading-nbt)
  - [Reading and Writing Options](#reading-and-writing-options)
- [Low-Level API](#low-level-api)
- [Breaking Changes in 0.2.0](#breaking-changes-in-020)
- [Building](#building)
- [Testing](#testing)
- [Roadmap](#roadmap)
- [Project Structure](#project-structure)
- [Motivation](#motivation)
- [License](#license)

## Features

- Modern C++23 implementation
- Support for all standard NBT tag types
- High-level `Nbt` object representation
- Binary NBT serialization
- Binary NBT parsing (under development)
- Nested compounds and lists
- Homogeneous list validation
- Byte, Int, and Long arrays
- Big-endian binary encoding
- Support for `std::vector<std::uint8_t>` and `std::span`
- Stream-based reading interface
- Configurable reading and writing interfaces
- Low-level tag reading and writing functions
- CMake integration with `FetchContent`
- GoogleTest-based test suite

## Supported NBT Tags

CastleNBT supports the 12 standard NBT value types.

| NBT Tag | C++ Type |
|---|---|
| `TAG_End` | Internal marker |
| `TAG_Byte` | `std::int8_t` |
| `TAG_Short` | `std::int16_t` |
| `TAG_Int` | `std::int32_t` |
| `TAG_Long` | `std::int64_t` |
| `TAG_Float` | `float` |
| `TAG_Double` | `double` |
| `TAG_Byte_Array` | `std::vector<std::int8_t>` |
| `TAG_String` | `std::string` |
| `TAG_List` | `std::vector<Nbt>` |
| `TAG_Compound` | `std::vector<Nbt>` |
| `TAG_Int_Array` | `std::vector<std::int32_t>` |
| `TAG_Long_Array` | `std::vector<std::int64_t>` |

`TAG_End` is used internally to terminate compound tags and is not intended to be created as a normal data value.

Both lists and compounds use `std::vector<Nbt>` internally. Their NBT tag identifiers distinguish their meaning and binary representation.

> [!NOTE]
> Writing support is implemented for the standard tag types. Reading support is being developed and tested for the upcoming 0.2.0 release.

## Requirements

- A C++23-compatible compiler
- CMake 3.24 or newer
- A standard library with the required C++23 features

GoogleTest is only required when building the test suite.

## Installation

### CMake FetchContent

CastleNBT can be included directly in another CMake project using `FetchContent`.

To use the development branch:

```cmake
include(FetchContent)

FetchContent_Declare(
    castleNBT
    GIT_REPOSITORY https://github.com/kronborg6/CastleNBT.git
    GIT_TAG dev
)

FetchContent_MakeAvailable(castleNBT)

target_link_libraries(your_target
    PRIVATE castleNBT
)
```

Include the library headers:

```cpp
#include <castle/nbt.hpp>
#include <castle/reader.hpp>
#include <castle/writer.hpp>
```

> [!IMPORTANT]
> The `dev` branch is not a stable release. For reproducible builds, use a release tag or commit hash.
>
> After 0.2.0 is released, replace `GIT_TAG dev` with the corresponding release tag.

### Git Submodule

CastleNBT can also be included as a Git submodule.

```bash
git submodule add -b dev https://github.com/kronborg6/CastleNBT.git external/CastleNBT

git submodule update --init --recursive
```

Add the library to your project:

```cmake
add_subdirectory(external/CastleNBT)

target_link_libraries(your_target
    PRIVATE castleNBT
)
```

## Quick Start

Create an NBT compound containing player information and serialize it.

```cpp
#include <castle/nbt.hpp>
#include <castle/writer.hpp>

#include <cstdint>
#include <vector>

using castle::nbt::Nbt;
using castle::nbt::NbtWriter;

int main() {
    auto root = Nbt::makeCompound("");

    root.add(Nbt::makeString("name", "Steve"));
    root.add(Nbt::makeInt("health", 20));
    root.add(Nbt::makeFloat("speed", 0.1f));

    std::vector<std::uint8_t> data =
        NbtWriter::writer(root);

    return 0;
}
```

`NbtWriter::writer()` serializes the NBT structure into binary data.

The example produces a root compound containing three named tags.

## Usage

### Creating NBT Tags

NBT values are represented by the `castle::nbt::Nbt` class.

Create individual tags using static factory methods:

```cpp
auto byteTag   = Nbt::makeByte("byte", 127);
auto shortTag  = Nbt::makeShort("short", 1000);
auto intTag    = Nbt::makeInt("int", 12345);
auto longTag   = Nbt::makeLong("long", 123456789LL);

auto floatTag  = Nbt::makeFloat("float", 0.5f);
auto doubleTag = Nbt::makeDouble("double", 0.75);

auto stringTag = Nbt::makeString("name", "CastleNBT");
```

Each `Nbt` object stores:

- Its NBT tag type
- Its name
- Its value

### Compounds

A `TAG_Compound` contains named NBT tags of different types.

```cpp
auto player = Nbt::makeCompound("player");

player.add(Nbt::makeString("name", "Steve"));
player.add(Nbt::makeInt("health", 20));
player.add(Nbt::makeLong("xp", 12345));
```

Compounds can contain other compounds.

```cpp
auto position = Nbt::makeCompound("position");

position.add(Nbt::makeDouble("x", 10.5));
position.add(Nbt::makeDouble("y", 64.0));
position.add(Nbt::makeDouble("z", -32.5));

player.add(std::move(position));
```

The resulting structure is conceptually equivalent to:

```snbt
{
    player: {
        name: "Steve",
        health: 20,
        xp: 12345L,
        position: {
            x: 10.5d,
            y: 64.0d,
            z: -32.5d
        }
    }
}
```

This is an illustration of the resulting data, not output from an SNBT serializer.

### Lists

A `TAG_List` contains unnamed elements of the same NBT type.

```cpp
auto numbers = Nbt::makeList("numbers");

numbers.append(std::int32_t{10});
numbers.append(std::int32_t{20});
numbers.append(std::int32_t{30});
```

Elements can also be created explicitly:

```cpp
auto numbers = Nbt::makeList("numbers");

numbers.append(Nbt::makeInt("", 10));
numbers.append(Nbt::makeInt("", 20));
numbers.append(Nbt::makeInt("", 30));
```

Lists can contain complex NBT values, including compounds.

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
```

CastleNBT validates that list elements have matching NBT tag types.

### Arrays

NBT defines three specialized array types.

**Byte Array**

```cpp
auto bytes = Nbt::makeByteArray(
    "bytes",
    {1, 2, 3, 4, 5}
);
```

**Int Array**

```cpp
auto integers = Nbt::makeIntArray(
    "integers",
    {100, 200, 300}
);
```

**Long Array**

```cpp
auto longs = Nbt::makeLongArray(
    "longs",
    {1000, 2000, 3000}
);
```

Unlike `TAG_List`, specialized arrays contain primitive numeric values rather than individual `Nbt` objects.

### Accessing NBT Values

CastleNBT provides accessors for the tag type, name, and underlying value.

```cpp
auto tag = Nbt::makeInt("health", 20);

auto type = tag.tag();
auto name = tag.name();

const auto& value = tag.value();
```

Values are stored in `NbtValue`, a `std::variant` containing the supported C++ value types.

Retrieve a specific value using `std::get`:

```cpp
std::int32_t health =
    std::get<std::int32_t>(tag.value());
```

CastleNBT also provides a tag-based getter:

```cpp
std::int32_t health =
    tag.get<castle::nbt::NbtTag::IntTag>();
```

The requested type must match the actual NBT tag type.

**Renaming tags**

```cpp
tag.rename("playerHealth");
```

Remove a tag name:

```cpp
tag.clearName();
```

Create a renamed copy:

```cpp
auto copy = tag.renamed("newHealth");
```

### Writing NBT

The high-level writing API is provided by `NbtWriter`.

```cpp
#include <castle/writer.hpp>
```

**Write to a new byte vector**

```cpp
auto data = NbtWriter::writer(root);
```

**Append to an existing byte vector**

```cpp
std::vector<std::uint8_t> data;

NbtWriter::writer(root, data);
```

The writer serializes NBT data using the standard big-endian binary format.

**Write to a file**

The current writer returns binary data that can be saved using standard C++ file I/O.

```cpp
#include <fstream>

auto data = NbtWriter::writer(root);

std::ofstream file("example.nbt", std::ios::binary);

file.write(
    reinterpret_cast<const char*>(data.data()),
    static_cast<std::streamsize>(data.size())
);
```

A dedicated file-writing interface is planned.

### Reading NBT

The new high-level reading API is provided by `NbtReader`.

```cpp
#include <castle/reader.hpp>
```

> [!CAUTION]
> The reader interface is present in `dev`, but its implementation is still being finalized. The following examples describe the intended public API and should be used after the reader fixes are completed.

**Read from binary data**

```cpp
std::vector<std::uint8_t> data = /* NBT binary data */;

Nbt root = NbtReader::reader(data);
```

The reader identifies the NBT tag types and constructs the corresponding `Nbt` objects.

**Read from a specific offset**

```cpp
std::size_t offset = 0;

Nbt root = NbtReader::reader(data, offset);
```

The offset-based overload allows callers to specify where reading begins and retrieve the resulting position.

**Read from a stream**

```cpp
#include <fstream>

std::ifstream file("example.nbt", std::ios::binary);

Nbt root = NbtReader::reader(file);
```

The stream-reading overload is intended to support reading NBT from files and other C++ input streams.

### Reading and Writing Options

CastleNBT provides option structures for customizing its high-level interfaces.

**WriteOptions**

```cpp
castle::nbt::WriteOptions options;

options.namelessRoot = false;
options.wrapNonCompoundRoot = true;

auto data = NbtWriter::writer(root, options);
```

Available options:

| Option | Default | Description |
|---|---|---|
| `namelessRoot` | `false` | Controls writing a root without a name field |
| `wrapNonCompoundRoot` | `true` | Wraps a non-compound root in a compound |

When non-compound root wrapping is enabled, CastleNBT creates a compound root containing the original tag.

If that tag has no name, the wrapper uses `"value"` as the child name.

**ReadOptions**

```cpp
castle::nbt::ReadOptions options;

options.namelessRoot = false;
options.allowTrailingBytes = false;
options.maxDepth = 512;
```

The reading options are declared in the current development API.

They are not yet fully applied by the reader implementation and should not be relied upon for validation or parsing behavior until completed.

## Low-Level API

In addition to `NbtReader` and `NbtWriter`, CastleNBT provides low-level functions for working with individual NBT tags.

The low-level interfaces are declared in:

```cpp
#include <castle/lowlevel/readTag.hpp>
#include <castle/lowlevel/writeTag.hpp>
```

Individual read functions include:

```cpp
FromTagByte(...)
FromTagShort(...)
FromTagInt(...)
FromTagLong(...)
FromTagFloat(...)
FromTagDouble(...)
FromTagByteArray(...)
FromTagString(...)
FromTagList(...)
FromTagCompound(...)
FromTagIntArray(...)
FromTagLongArray(...)
```

These functions operate on binary data and support offset-based parsing.

Corresponding low-level write functions are also available.

The reading API uses `std::span<const std::uint8_t>` to reference binary data without requiring ownership of the buffer.

> [!WARNING]
> The low-level API is experimental. Its interfaces, names, and behavior may change before 0.2.0.
>
> The direct-value reading helpers are still being developed and should not yet be considered stable.

## Breaking Changes in 0.2.0

The upcoming 0.2.0 release introduces significant API changes compared with 0.1.0.

### 1. Serialization moved to NbtWriter

In version 0.1.0, serialization was performed through the `Nbt` object:

```cpp
auto data = root.compile();
```

The development branch uses a separate writer interface:

```cpp
auto data = NbtWriter::writer(root);
```

The old `compile()` methods are no longer part of the active public interface.

### 2. New reader interface

The development branch introduces `NbtReader` for parsing binary NBT data into `Nbt` objects.

```cpp
Nbt root = NbtReader::reader(data);
```

This functionality was not available in 0.1.0.

### 3. Templated container operations

The `add()` and `append()` methods now use templates.

This allows callers to work with `Nbt` objects and supported underlying value types.

```cpp
compound.add(Nbt::makeInt("health", 20));

list.append(std::int32_t{10});
```

### 4. New typed value accessor

The `Nbt` class provides a tag-based getter:

```cpp
auto value = tag.get<NbtTag::IntTag>();
```

The existing `value()` method remains available.

### 5. Reading and writing configuration

The new high-level APIs introduce `ReadOptions` and `WriteOptions`.

These provide a place for configuration without changing every reading or writing function signature.

### 6. Compatibility

CastleNBT is still in the `0.x` development series.

Breaking API changes may occur between releases as the library evolves.

Users upgrading from 0.1.0 should review the new reader, writer, and container interfaces before updating.

## Building

Clone the repository and switch to the development branch.

```bash
git clone https://github.com/kronborg6/CastleNBT.git

cd CastleNBT

git switch dev
```

Configure CMake:

```bash
cmake -S . -B build
```

Build the library:

```bash
cmake --build build
```

When built as the top-level project, CastleNBT also builds the development executable:

```text
castleNBTTool
```

## Testing

CastleNBT uses GoogleTest for automated testing.

Configure the project with testing enabled:

```bash
cmake -S . -B build -DBUILD_TESTING=ON
```

Build:

```bash
cmake --build build
```

Run all registered tests:

```bash
ctest --test-dir build --output-on-failure
```

Testing covers NBT serialization and other library behavior as the implementation develops.

Additional reader, round-trip, validation, and error-handling tests are planned for the 0.2.0 release.

## Roadmap

### 0.2.0 — Reading and Writing API

The main development goals for version 0.2.0 are:

- [x] Separate high-level writing interface
- [x] Introduce high-level reading interface
- [x] Implement binary tag readers
- [x] Support offset-based reading
- [x] Introduce reading and writing options
- [ ] Complete reader validation and option handling
- [ ] Complete direct-value low-level readers
- [ ] Expand reader and round-trip tests
- [ ] Finalize the 0.2.0 API

### Future Development

- [ ] Dedicated NBT file reading and writing APIs
- [ ] NBT to SNBT conversion
- [ ] SNBT parsing
- [ ] Human-readable NBT tree output
- [ ] Parsing human-readable NBT
- [ ] Gzip and Zlib compression support
- [ ] Additional validation and error reporting

See [TODO.md](TODO.md) for development notes.

## Project Structure

```text
CastleNBT/
├── include/
│   └── castle/
│       ├── nbt.hpp
│       ├── reader.hpp
│       ├── writer.hpp
│       └── lowlevel/
│           ├── common.hpp
│           ├── readTag.hpp
│           └── writeTag.hpp
│
├── src/
│   ├── createNbt.cpp
│   ├── endian.cpp
│   ├── variable.cpp
│   ├── writeNbt.cpp
│   ├── writeTag.cpp
│   ├── readTag.cpp
│   ├── writer.cpp
│   ├── reader.cpp
│   └── nbt.cpp
│
├── tests/
├── tools/
├── CMakeLists.txt
├── LICENSE
├── README.md
└── TODO.md
```

## Motivation

CastleNBT was created as part of CastleCraft, a personal project to build a Minecraft server from scratch in C++.

Rather than using an existing NBT library, the goal is to implement the format directly and gain a deeper understanding of:

- Binary serialization and parsing
- Byte ordering and endianness
- Binary file formats
- Recursive data structures
- Modern C++ library design
- Memory and data representation
- The Minecraft protocol and related data formats
- Automated testing and build systems

CastleNBT is both a usable library project and an opportunity to explore modern C++ and systems programming.

## License

CastleNBT is licensed under the [MIT License](LICENSE).
