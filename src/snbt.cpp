#include "snbt.hpp"
#include "castle/nbt.hpp"
#include "castle/writer.hpp"
#include <cstddef>
#include <cstdint>
#include <format>
#include <string>
#include <vector>

namespace {
  void addTabs(std::string& output, castle::nbt::SnbtOptions options) {
    output += std::string(options.indentSize * options.deph, ' ');
  }
} // namespace
namespace castle::nbt::snbt {
  void addKey(std::string& output, const Nbt& tag);
  void FromTagListSNBT(std::string& output, const Nbt& listTag, SnbtOptions options);
  void addArray(std::string& output, const Nbt& array, SnbtOptions options);
  // void FromTagCompoundSNBT(std::string& output, const Nbt& tag, WriteOptions options);
  std::string snbt(const Nbt& root, SnbtOptions options) {
    std::string result;

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
      options.deph++;
      addTabs(result, options);
      FromTagCompoundSNBT(result, root, options);
      options.deph--;
      result += "\n}";
      break;
    case castle::nbt::NbtTag::IntArrayTag:
      break;
    case castle::nbt::NbtTag::LongArrayTag:
      break;
    }

    return result;
  }

  void FromTagCompoundSNBT(std::string& output, const Nbt& tag, SnbtOptions options) {
    const auto& values = tag.get<NbtTag::CompoundTag>();
    for (const auto& value : values) {
      // addTabs(output, options.tabcout);

      switch (value.tag()) {
      case castle::nbt::NbtTag::EndTag:
        output += "}";
        break;
      case castle::nbt::NbtTag::ByteTag:
        addKey(output, value);
        output += std::format(": {}b", static_cast<int>(value.get<NbtTag::ByteTag>()));
        break;
      case castle::nbt::NbtTag::ShortTag:
        addKey(output, value);
        output += std::format(": {}s", value.get<NbtTag::ShortTag>());
        break;
      case castle::nbt::NbtTag::IntTag:
        addKey(output, value);
        output += std::format(": {}", value.get<NbtTag::IntTag>());
        break;
      case castle::nbt::NbtTag::LongTag:
        addKey(output, value);
        output += std::format(": {}l", value.get<NbtTag::LongTag>());
        break;
      case castle::nbt::NbtTag::FloatTag:
        addKey(output, value);
        output += std::format(": {}f", value.get<NbtTag::FloatTag>());
        break;
      case castle::nbt::NbtTag::DoubleTag:
        addKey(output, value);
        output += std::format(": {}d", value.get<NbtTag::DoubleTag>());
        break;
      case castle::nbt::NbtTag::ByteArrayTag:
        addKey(output, value);
        output += std::format(": [\n");
        options.deph++;
        addArray(output, value, options);
        output += "\n";
        options.deph--;
        addTabs(output, options);
        output += "]";
        break;
      case castle::nbt::NbtTag::StringTag:
        addKey(output, value);
        output += std::format(": \"{}\"", value.get<NbtTag::StringTag>());
        break;
      case castle::nbt::NbtTag::ListTag:
        addKey(output, value);
        output += std::format(": [\n");
        options.deph++;
        addTabs(output, options);
        FromTagListSNBT(output, value, options);
        output += "\n";
        options.deph--;
        addTabs(output, options);
        output += "]";
        break;
      case castle::nbt::NbtTag::CompoundTag:
        addKey(output, value);
        output += std::format(": {{\n");
        // output += "{\n";
        options.deph++;
        addTabs(output, options);
        FromTagCompoundSNBT(output, value, options);

        output += "\n";
        options.deph--;
        addTabs(output, options);
        output += "}";

        break;
      case castle::nbt::NbtTag::IntArrayTag:
        addKey(output, value);
        output += std::format(": [\n");
        options.deph++;
        addArray(output, value, options);
        output += "\n";
        options.deph--;
        addTabs(output, options);
        output += "]";
        break;
      case castle::nbt::NbtTag::LongArrayTag:
        addKey(output, value);
        output += std::format(": [\n");
        options.deph++;
        addArray(output, value, options);
        output += "\n";
        options.deph--;
        addTabs(output, options);
        output += "]";
        break;
      }

      if (&value != &values.back()) {
        output += ",";
        output += "\n";
      }
      addTabs(output, options);
    }
  }
  void FromTagListSNBT(std::string& output, const Nbt& listTag, SnbtOptions options) {

    const auto& values = listTag.get<NbtTag::ListTag>();

    for (const auto& value : values) {
      switch (value.tag()) {
      case castle::nbt::NbtTag::EndTag:
        output += "}";
        break;
      case castle::nbt::NbtTag::ByteTag:
        output += std::format("{}b", static_cast<int>(value.get<NbtTag::ByteTag>()));
        break;
      case castle::nbt::NbtTag::ShortTag:
        output += std::format("{}s", value.get<NbtTag::ShortTag>());
        break;
      case castle::nbt::NbtTag::IntTag:
        output += std::format("{}", value.get<NbtTag::IntTag>());
        break;
      case castle::nbt::NbtTag::LongTag:
        output += std::format("{}l", value.get<NbtTag::LongTag>());
        break;
      case castle::nbt::NbtTag::FloatTag:
        output += std::format("{}f", value.get<NbtTag::FloatTag>());
        break;
      case castle::nbt::NbtTag::DoubleTag:
        output += std::format("{}d", value.get<NbtTag::DoubleTag>());
        break;
      case castle::nbt::NbtTag::ByteArrayTag:
        output += std::format(": [\n");
        options.deph++;
        addArray(output, value, options);
        output += "\n";
        options.deph--;
        addTabs(output, options);
        output += "]";
        break;
      case castle::nbt::NbtTag::StringTag:
        output += std::format("\"{}\"", value.get<NbtTag::StringTag>());
        break;
      case castle::nbt::NbtTag::ListTag:
        output += std::format(": [\n");
        options.deph++;
        addTabs(output, options);
        FromTagListSNBT(output, value, options);
        output += "\n";
        options.deph--;
        addTabs(output, options);
        output += "]";
        break;
      case castle::nbt::NbtTag::CompoundTag:
        output += std::format("{{\n");
        // output += "{\n";
        options.deph++;
        addTabs(output, options);
        FromTagCompoundSNBT(output, value, options);

        output += "\n";
        options.deph--;
        addTabs(output, options);
        output += "}";

        break;
      case castle::nbt::NbtTag::IntArrayTag:
        output += std::format(": [\n");
        options.deph++;
        addArray(output, value, options);
        output += "\n";
        options.deph--;
        addTabs(output, options);
        output += "]";
        break;

      case castle::nbt::NbtTag::LongArrayTag:
        output += std::format(": [\n");
        options.deph++;
        addArray(output, value, options);
        output += "\n";
        options.deph--;
        addTabs(output, options);
        output += "]";
        break;
      }

      if (&value != &values.back()) {
        output += ",";
        output += "\n";
      }

      addTabs(output, options);
    }
  }

  void addArray(std::string& output, const Nbt& array, SnbtOptions options) {
    switch (array.tag()) {
    case NbtTag::ByteArrayTag: {

      const auto& values = array.get<NbtTag::ByteArrayTag>();

      for (std::size_t i = 0; i < values.size(); i++) {
        addTabs(output, options);
        output += std::format("{}b", std::to_string(values[i]));
        if (i + 1 != values.size()) {
          output += ",";
          output += "\n";
        }
      }

      // addTabs(output, options);
    } break;
    case NbtTag::IntArrayTag: {

      const std::vector<std::int32_t>& values = array.get<NbtTag::IntArrayTag>();

      for (std::size_t i = 0; i < values.size(); i++) {
        addTabs(output, options);
        output += std::format("{}", std::to_string(values[i]));
        if (i + 1 != values.size()) {
          output += ",";
          output += "\n";
        }
      }
    } break;
    case NbtTag::LongArrayTag: {

      const std::vector<std::int64_t>& values = array.get<NbtTag::LongArrayTag>();
      for (std::size_t i = 0; i < values.size(); i++) {
        addTabs(output, options);
        output += std::format("{}l", std::to_string(values[i]));
        if (i + 1 != values.size()) {
          output += ",";
          output += "\n";
        }
      }
    } // namespace castle::nbt::snbt
    break;
    default:

      break;
    }
  } // namespace castle::nbt::snbt

  void addKey(std::string& output, const Nbt& tag) {
    // i no this lambda is not need but i just wan't to make one
    auto key = [&]() -> std::string {
      if (tag.name().contains(' ')) {
        return std::format("\"{}\"", tag.name());
      }
      return std::string(tag.name());
    };

    output += key();
  }
} // namespace castle::nbt::snbt
