#include "castle/writer.hpp"
#include "castle/lowlevel/common.hpp"
#include "castle/lowlevel/writeTag.hpp"
#include "castle/nbt.hpp"
#include "castle/reader.hpp"
#include "snbt.hpp"
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include <filesystem>
#include <fstream>
#include <ios>
namespace castle::nbt {

  constexpr std::string_view kDefaultWrapKey = "value";

  void writeRoot(const Nbt& node, std::vector<uint8_t>& data, WriteOptions options) {

    if (node.tag() == NbtTag::EndTag) {
      throw std::runtime_error("root tag can't be a endTag");
    }
    TagContext ctx = options.namelessRoot ? TagContext::NamelessRoot : TagContext::Named;
    if (node.tag() != NbtTag::CompoundTag && options.wrapNonCompoundRoot) {

      writeNbt(Nbt::makeCompound("",
                                 std::vector<Nbt>{node.name().empty()
                                                      ? node.renamed(std::string(kDefaultWrapKey))
                                                      : node}),
               data,
               ctx);
    } else {
      if (options.wrapNonCompoundRoot && options.namelessRoot) {

        writeNbt(node, data, TagContext::NamelessRootAndNonCompoundRoot);
        if (options.namelessRoot) {

        } else {

          writeNbt(node, data, TagContext::NonCompoundRoot);
        }
        // data.erase(data.begin(), data.begin() + 1);
      } else if (options.wrapNonCompoundRoot) {
        writeNbt(node, data, TagContext::NonCompoundRoot);
      } else if (options.namelessRoot) {

        writeNbt(node, data, ctx);
      } else {

        writeNbt(node, data, ctx);
      }
    }
  }

  void NbtWriter::writer(const Nbt& tag, std::vector<std::uint8_t>& data, WriteOptions options) {
    writeRoot(tag, data, options);
  }
  std::vector<std::uint8_t> NbtWriter::writer(const Nbt& tag, WriteOptions options) {

    std::vector<std::uint8_t> output;

    writeRoot(tag, output, options);
    return output;
  }
  void NbtWriter::writer(const Nbt& tag, const std::string& path, WriteOptions options) {
    std::vector<std::uint8_t> output;
    writeRoot(tag, output, options);

    std::ofstream file(path, std::ios::binary);

    if (!file) {
      throw std::runtime_error("Failed to open file: " + path);
    }

    file.write(reinterpret_cast<const char*>(output.data()),
               static_cast<std::streamsize>(output.size()));

    file.close();

    if (!file) {
      throw std::runtime_error("Failed to write file: " + path);
    }
  }

  std::string NbtWriter::snbtWriter(const Nbt& tag, SnbtOptions options) {
    return snbt::snbt(tag, options);
  }
  std::string NbtWriter::snbtWriter(std::span<const uint8_t> rawNbt, SnbtOptions options) {
    Nbt root = NbtReader::reader(rawNbt, {.namelessRoot = true});

    return snbt::snbt(root, options);
  }
  void NbtWriter::snbtWriter(const Nbt& tag, const std::string& path, SnbtOptions options) {
    std::string snbt = snbt::snbt(tag, options);

    std::ofstream file(path, std::ios::binary);

    if (!file) {
      throw std::runtime_error("Failed to open file: " + path);
    }

    file.write(reinterpret_cast<const char*>(snbt.data()),
               static_cast<std::streamsize>(snbt.size()));

    file.close();

    if (!file) {
      throw std::runtime_error("Failed to write file: " + path);
    }
  }
  void NbtWriter::snbtWriter(std::span<const uint8_t> rawNbt,
                             const std::string& path,
                             SnbtOptions options) {

    Nbt tag = NbtReader::reader(rawNbt, {.namelessRoot = true});
    std::string snbt = snbt::snbt(tag, options);
    std::ofstream file(path, std::ios::binary);

    if (!file) {
      throw std::runtime_error("Failed to open file: " + path);
    }

    file.write(reinterpret_cast<const char*>(snbt.data()),
               static_cast<std::streamsize>(snbt.size()));

    file.close();

    if (!file) {
      throw std::runtime_error("Failed to write file: " + path);
    }
  }

} // namespace castle::nbt
