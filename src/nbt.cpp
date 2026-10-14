#include "endian.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <nbt/nbt.hpp>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

namespace minecraft::nbt {
  void writeNbt(const Nbt& node, std::vector<uint8_t>& data) {
    switch (node.tag) {
    case NbtTag::EndTag:
      ToEndTag(data);
      break;
    case NbtTag::ByteTag:
      ToTagByte(node.name.value_or(""), std::get<int8_t>(node.value), data, node.context);
      break;
    case NbtTag::ShortTag:
      ToTagShort(node.name.value_or(""), std::get<int16_t>(node.value), data, node.context);
      break;
    case NbtTag::IntTag:
      ToTagInt(node.name.value_or(""), std::get<int32_t>(node.value), data, node.context);
      break;
    case NbtTag::LongTag:
      ToTagLong(node.name.value_or(""), std::get<int64_t>(node.value), data, node.context);
      break;
    case NbtTag::FloatTag:
      ToTagFloat(node.name.value_or(""), std::get<float>(node.value), data, node.context);
      break;
    case NbtTag::DoubleTag:
      ToTagDouble(node.name.value_or(""), std::get<double>(node.value), data, node.context);
      break;
    case NbtTag::ByteArrayTag:
      break;
    case NbtTag::StringTag: {
      const std::string& value = std::get<std::string>(node.value);

      ToTagString(node.name.value_or(""), value, data, node.context);

      break;
    }
    case NbtTag::ListTag: {
      const std::vector<Nbt>& values = std::get<std::vector<Nbt>>(node.value);

      bool allSameTag =
          values.empty() || std::all_of(values.begin(), values.end(), [&](const Nbt& u) {
            return u.tag == values.front().tag;
          });

      if (!allSameTag) {
        throw std::runtime_error("not all the same tag");
      }

      NbtTag valueType = !values.empty() ? values.front().tag : NbtTag::EndTag;

      ToTagList(node.name.value_or(""), valueType, values.size(), data, node.context);

      for (const Nbt& value : values) {
        Nbt listValue = value;

        listValue.context = TagContext::ListElement;

        writeNbt(listValue, data);
      }

      break;
    }
    case NbtTag::CompoundTag: {
      ToTagCompound(node.name.value_or(""), data, node.context);

      const std::vector<Nbt>& values = std::get<std::vector<Nbt>>(node.value);

      for (const Nbt& var : values) {
        writeNbt(var, data);
      }
      ToEndTag(data);

      break;
    }
    case NbtTag::IntArrayTag:
      break;
    case NbtTag::LongArrayTag:
      break;
    }
  }
} // namespace minecraft::nbt
