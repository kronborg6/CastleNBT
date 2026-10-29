#include "snbt.hpp"
#include "castle/nbt.hpp"
#include "castle/writer.hpp"
#include <cstdint>
#include <format>
#include <string>

namespace {
  void addTabs(std::string& output, std::uint32_t tabs) {
    for (std::uint32_t i = 0; i < tabs; i++) {
      output += "\t";
    }
  }
} // namespace
namespace castle::nbt::snbt {

  // void FromTagCompoundSNBT(std::string& output, const Nbt& tag, WriteOptions options);
  std::string snbt(const Nbt& root, WriteOptions options) {
    std::string result;

    snbt::Options snbtOption = {options

    };
    switch (root.tag()) {
    case castle::nbt::NbtTag::EndTag:
      break;
    case castle::nbt::NbtTag::ByteTag:
      break;
    case castle::nbt::NbtTag::ShortTag:
      break;
    case castle::nbt::NbtTag::IntTag:
      break;
    case castle::nbt::NbtTag::LongTag:
      break;
    case castle::nbt::NbtTag::FloatTag:
      break;
    case castle::nbt::NbtTag::DoubleTag:
      break;
    case castle::nbt::NbtTag::ByteArrayTag:
      break;
    case castle::nbt::NbtTag::StringTag:
      break;
    case castle::nbt::NbtTag::ListTag:
      break;
    case castle::nbt::NbtTag::CompoundTag:
      result += "{\n";
      addTabs(result, ++snbtOption.tabcout);
      FromTagCompoundSNBT(result, root, snbtOption);
      snbtOption.tabcout--;
      result += "\n}";
      break;
    case castle::nbt::NbtTag::IntArrayTag:
      break;
    case castle::nbt::NbtTag::LongArrayTag:
      break;
    }

    return result;
  }

  void FromTagCompoundSNBT(std::string& output, const Nbt& tag, Options options) {
    if (!options.options.namelessRoot) {
    }
    const auto& values = tag.get<NbtTag::CompoundTag>();
    for (const auto& value : values) {
      // addTabs(output, options.tabcout);

      switch (value.tag()) {
      case castle::nbt::NbtTag::EndTag:
        break;
      case castle::nbt::NbtTag::ByteTag:
        break;
      case castle::nbt::NbtTag::ShortTag:
        break;
      case castle::nbt::NbtTag::IntTag:
        output += std::format("\"{}\": {}", value.name(), value.get<NbtTag::IntTag>());
        break;
      case castle::nbt::NbtTag::LongTag:
        break;
      case castle::nbt::NbtTag::FloatTag:
        break;
      case castle::nbt::NbtTag::DoubleTag:
        break;
      case castle::nbt::NbtTag::ByteArrayTag:
        break;
      case castle::nbt::NbtTag::StringTag:
        output += std::format("\"{}\": \"{}\"", value.name(), value.get<NbtTag::StringTag>());
        break;
      case castle::nbt::NbtTag::ListTag:
        break;
      case castle::nbt::NbtTag::CompoundTag:
        output += std::format("\"{}\": {{\n", value.name());
        // output += "{\n";
        addTabs(output, ++options.tabcout);
        FromTagCompoundSNBT(output, value, options);

        output += "\n";
        addTabs(output, --options.tabcout);
        output += "}";

        break;
      case castle::nbt::NbtTag::IntArrayTag:
        break;
      case castle::nbt::NbtTag::LongArrayTag:
        break;
      }

      if (&value != &values.back()) {
        output += ",";
        output += "\n";
      }
      addTabs(output, options.tabcout);
    }
  }
} // namespace castle::nbt::snbt
