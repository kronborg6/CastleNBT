#include "castle/nbt.hpp"
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

namespace castle::nbt {
  void writeNbt(const Nbt& node, std::vector<uint8_t>& data, TagContext context) {
    switch (node.tag) {
    case NbtTag::EndTag:
      ToEndTag(data);
      break;
    case NbtTag::ByteTag:
      ToTagByte(node.name.value_or(""), std::get<int8_t>(node.value), data, context);
      break;
    case NbtTag::ShortTag:
      ToTagShort(node.name.value_or(""), std::get<int16_t>(node.value), data, context);
      break;
    case NbtTag::IntTag:
      ToTagInt(node.name.value_or(""), std::get<int32_t>(node.value), data, context);
      break;
    case NbtTag::LongTag:
      ToTagLong(node.name.value_or(""), std::get<int64_t>(node.value), data, context);
      break;
    case NbtTag::FloatTag:
      ToTagFloat(node.name.value_or(""), std::get<float>(node.value), data, context);
      break;
    case NbtTag::DoubleTag:
      ToTagDouble(node.name.value_or(""), std::get<double>(node.value), data, context);
      break;
    case NbtTag::ByteArrayTag: {
      const std::vector<int8_t>& values = std::get<std::vector<int8_t>>(node.value);
      ToTagByteArray(node.name.value_or(""), values, data, context);
      break;
    }
    case NbtTag::StringTag: {
      const std::string& value = std::get<std::string>(node.value);

      ToTagString(node.name.value_or(""), value, data, context);

      break;
    }
    case NbtTag::ListTag: {
      const std::vector<Nbt>& values = std::get<std::vector<Nbt>>(node.value);

      bool allSameTag =
          values.empty() || std::all_of(values.begin() + 1, values.end(), [&](const Nbt& u) {
            return u.tag == values.front().tag;
          });

      if (!allSameTag) {
        throw std::runtime_error("not all the same tag");
      }

      NbtTag valueType = !values.empty() ? values.front().tag : NbtTag::EndTag;

      ToTagList(node.name.value_or(""), valueType, values.size(), data, context);

      for (const Nbt& value : values) {

        writeNbt(value, data, TagContext::ListElement);
      }

      break;
    }
    case NbtTag::CompoundTag: {
      ToTagCompound(node.name.value_or(""), data, context);

      const std::vector<Nbt>& values = std::get<std::vector<Nbt>>(node.value);

      for (const Nbt& var : values) {
        writeNbt(var, data);
      }
      ToEndTag(data);

      break;
    }
    case NbtTag::IntArrayTag: {

      const std::vector<int32_t>& values = std::get<std::vector<int32_t>>(node.value);
      ToTagIntArray(node.name.value_or(""), values, data, context);
      break;
    }
    case NbtTag::LongArrayTag: {

      const std::vector<int64_t>& values = std::get<std::vector<int64_t>>(node.value);
      ToTagLongArray(node.name.value_or(""), values, data, context);
      break;
    }
    default:
      throw std::runtime_error("not a vailed tag type");
      break;
    }
  }

  Nbt makeCompound(std::string name) {
    return Nbt{
        .tag = NbtTag::CompoundTag,
        .name = name,
        .value = std::vector<Nbt>{},
    };
  }

  Nbt makeString(std::string name, std::string value) {
    return Nbt{
        .tag = NbtTag::StringTag,
        .name = name,
        .value = value,
    };
  }
} // namespace castle::nbt
