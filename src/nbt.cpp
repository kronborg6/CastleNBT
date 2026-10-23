#include "castle/nbt.hpp"
#include "castle/lowlevel/common.hpp"
#include "castle/lowlevel/readTag.hpp"
#include "castle/lowlevel/writeTag.hpp"
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

namespace castle::nbt {

  void writeNbt(const Nbt& node, std::vector<uint8_t>& data, TagContext context) {
    switch (node.tag()) {
    case NbtTag::EndTag:
      ToEndTag(data);
      break;
    case NbtTag::ByteTag:
      ToTagByte(node.name(), std::get<int8_t>(node.value()), data, context);
      break;
    case NbtTag::ShortTag:
      ToTagShort(node.name(), std::get<int16_t>(node.value()), data, context);
      break;
    case NbtTag::IntTag:
      ToTagInt(node.name(), std::get<int32_t>(node.value()), data, context);
      break;
    case NbtTag::LongTag:
      ToTagLong(node.name(), std::get<int64_t>(node.value()), data, context);
      break;
    case NbtTag::FloatTag:
      ToTagFloat(node.name(), std::get<float>(node.value()), data, context);
      break;
    case NbtTag::DoubleTag:
      ToTagDouble(node.name(), std::get<double>(node.value()), data, context);
      break;
    case NbtTag::ByteArrayTag: {
      const std::vector<int8_t>& values = std::get<std::vector<int8_t>>(node.value());
      ToTagByteArray(node.name(), values, data, context);
      break;
    }
    case NbtTag::StringTag: {
      const std::string& value = std::get<std::string>(node.value());

      ToTagString(node.name(), value, data, context);

      break;
    }
    case NbtTag::ListTag: {
      const std::vector<Nbt>& values = std::get<std::vector<Nbt>>(node.value());

      bool allSameTag =
          values.empty() || std::all_of(values.begin() + 1, values.end(), [&](const Nbt& u) {
            return u.tag() == values.front().tag();
          });

      if (!allSameTag) {
        throw std::runtime_error("not all the same tag");
      }

      NbtTag valueType = !values.empty() ? values.front().tag() : NbtTag::EndTag;

      ToTagList(node.name(), valueType, values.size(), data, context);

      for (const Nbt& value : values) {

        writeNbt(value, data, TagContext::ListElement);
      }

      break;
    }
    case NbtTag::CompoundTag: {
      ToTagCompound(node.name(), data, context);

      const std::vector<Nbt>& values = std::get<std::vector<Nbt>>(node.value());

      for (const Nbt& var : values) {
        writeNbt(var, data);
      }
      ToEndTag(data);

      break;
    }
    case NbtTag::IntArrayTag: {

      const std::vector<int32_t>& values = std::get<std::vector<int32_t>>(node.value());
      ToTagIntArray(node.name(), values, data, context);
      break;
    }
    case NbtTag::LongArrayTag: {

      const std::vector<int64_t>& values = std::get<std::vector<int64_t>>(node.value());
      ToTagLongArray(node.name(), values, data, context);
      break;
    }
    default:
      throw std::runtime_error("not a vailed tag type");
      break;
    }
  }

  castle::nbt::Nbt::Nbt(std::string name, castle::nbt::NbtTag tag, NbtValue value)
      : _name(std::move(name)), _tag(std::move(tag)), _value(std::move(value)) {
    _init = true;
  };

  // void castle::nbt::Nbt::compile(std::vector<std::uint8_t>& output) {
  //   if (_tag != NbtTag::CompoundTag) {
  //     writeNbtDocument(*this, output);
  //   } else {
  //     writeNbt(*this, output);
  //   }
  // }

  // std::vector<std::uint8_t> castle::nbt::Nbt::compile() {
  //   std::vector<std::uint8_t> output;
  //   compile(output);
  //   return output;
  // }

} // namespace castle::nbt
