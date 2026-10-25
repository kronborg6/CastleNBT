#include "castle/lowlevel/readTag.hpp"
#include "castle/lowlevel/common.hpp"
#include "endian.hpp"
#include <algorithm>
#include <bit>
#include <castle/nbt.hpp>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <format>
#include <iostream>
#include <span>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <utility>
#include <vector>

#include <string>
#include <vector>

namespace castle::nbt {
  inline void
  requireBytes(std::span<const std::uint8_t> data, std::size_t offset, std::size_t count) {
    if (offset > data.size() || data.size() - offset < count) {
      throw std::runtime_error("unexpected end of data");
    }
  }

  std::string readTagHeader(std::span<const std::uint8_t> data,
                            std::size_t& offset,
                            NbtTag exepted,
                            nbt::TagContext context) {
    if (offset >= data.size()) {
      throw std::runtime_error("unexpected end of data");
    }
    if (context == TagContext::NonCompoundRoot) {
      return "";
    }

    if (context == TagContext::NamelessRoot) {
      if (static_cast<NbtTag>(data[offset++]) != exepted) {
        throw std::runtime_error("unexpected tag type");
      }
      return ""; // no name field follows
    }

    if (context == TagContext::NamelessRootAndNonCompoundRoot) {
      offset++;
      return "";
    }

    if (context == TagContext::ListElement) {
      return "";
    }
    if (static_cast<NbtTag>(data[offset++]) != exepted) {
      throw std::runtime_error("unexpected tag type");
    }
    if (exepted == NbtTag::EndTag) {
      return "";
    }
    auto nameLength = endian::readBigEndian<std::uint16_t>(data, offset);
    if (data.size() - offset < nameLength) {
      throw std::runtime_error("unexpected end of data");
    }
    std::string name(data.begin() + offset, data.begin() + offset + nameLength);
    offset += nameLength;
    return name;
  }

  void ToEndTag(std::vector<uint8_t>& data);

  Nbt FromTagByte(std::span<const std::uint8_t> data,
                  std::size_t& offset,
                  nbt::TagContext context) {
    std::string name = readTagHeader(data, offset, NbtTag::ByteTag, context);

    if (offset >= data.size()) {
      throw std::runtime_error("unexpected end of data");
    }
    std::int8_t value = static_cast<std::int8_t>(data[offset++]);

    return Nbt::makeByte(std::move(name), value);
  }

  Nbt FromTagShort(std::span<const std::uint8_t> data,
                   std::size_t& offset,
                   nbt::TagContext context) {
    std::string name = readTagHeader(data, offset, NbtTag::ShortTag, context);

    int16_t value = static_cast<int16_t>(endian::readBigEndian<uint16_t>(data, offset));

    return Nbt::makeShort(std::move(name), value);
  }

  Nbt FromTagInt(std::span<const std::uint8_t> data, std::size_t& offset, nbt::TagContext context) {
    std::string name = readTagHeader(data, offset, NbtTag::IntTag, context);

    int32_t value = static_cast<int32_t>(endian::readBigEndian<uint32_t>(data, offset));

    return Nbt::makeInt(std::move(name), value);
  }

  Nbt FromTagLong(std::span<const std::uint8_t> data,
                  std::size_t& offset,
                  nbt::TagContext context) {
    std::string name = readTagHeader(data, offset, NbtTag::LongTag, context);

    int64_t value = static_cast<int64_t>(endian::readBigEndian<uint64_t>(data, offset));

    return Nbt::makeLong(std::move(name), value);
  }

  Nbt FromTagFloat(std::span<const std::uint8_t> data,
                   std::size_t& offset,
                   nbt::TagContext context) {
    std::string name = readTagHeader(data, offset, NbtTag::FloatTag, context);

    uint32_t rawFloat = endian::readBigEndian<uint32_t>(data, offset);

    float value = endian::uint32ToFloat(rawFloat);

    return Nbt::makeFloat(std::move(name), value);
  }

  Nbt FromTagDouble(std::span<const std::uint8_t> data,
                    std::size_t& offset,
                    nbt::TagContext context) {
    std::string name = readTagHeader(data, offset, NbtTag::DoubleTag, context);

    uint64_t rawDouble = endian::readBigEndian<uint64_t>(data, offset);

    double value = endian::uint64ToDouble(rawDouble);

    return Nbt::makeDouble(std::move(name), value);
  }

  Nbt FromTagByteArray(std::span<const uint8_t> data,
                       std::size_t& offset,
                       nbt::TagContext context) {
    std::string name = readTagHeader(data, offset, NbtTag::ByteArrayTag, context);

    const size_t size = static_cast<size_t>(endian::readBigEndian<uint32_t>(data, offset));

    requireBytes(data, offset, size);

    std::vector<int8_t> values(data.begin() + offset, data.begin() + size + offset);
    offset += size;

    return Nbt::makeByteArray(std::move(name), std::move(values));
  }

  Nbt FromTagString(std::span<const std::uint8_t> data,
                    std::size_t& offset,
                    nbt::TagContext context) {
    std::string name = readTagHeader(data, offset, NbtTag::StringTag, context);

    uint16_t valueLengh = endian::readBigEndian<uint16_t>(data, offset);

    requireBytes(data, offset, valueLengh);
    std::string value(data.begin() + offset, data.begin() + valueLengh + offset);
    offset += valueLengh;

    return Nbt::makeString(std::move(name), std::move(value));
  }

  Nbt FromTagList(std::span<const uint8_t> data,
                  std::size_t& offset,
                  nbt::TagContext context,
                  std::size_t depth,
                  std::size_t maxDepth) {

    if (depth > maxDepth) {
      throw std::runtime_error("amx Depth reached");
    }

    std::string name = readTagHeader(data, offset, NbtTag::ListTag, context);

    if (offset >= data.size()) {
      throw std::runtime_error("unexpected end of data");
    }
    NbtTag listType = static_cast<NbtTag>(data[offset++]);
    NbtTag lastType = listType;

    std::uint32_t listLength =
        static_cast<std::uint32_t>(endian::readBigEndian<std::uint32_t>(data, offset));

    requireBytes(data, offset, listLength);

    std::vector<Nbt> values;
    values.reserve(listLength);

    for (std::uint32_t i = 0; i < listLength; i++) {

      if (listType != lastType) {
        throw std::runtime_error(std::format(
            "exepted: {} found: {}", std::to_underlying(listType), std::to_underlying(lastType)));
      }

      switch (listType) {
      case NbtTag::EndTag:
        if (listLength > 0) {
          throw std::runtime_error("list can't have Length when type is endTag");
        }
        return Nbt::makeList(std::move(name), std::move(values));
      case NbtTag::ByteTag:
        lastType = NbtTag::ByteTag;
        values.push_back(FromTagByte(data, offset, TagContext::ListElement));
        break;
      case NbtTag::ShortTag:
        lastType = NbtTag::ShortTag;
        values.push_back(FromTagShort(data, offset, TagContext::ListElement));
        break;
      case NbtTag::IntTag:
        lastType = NbtTag::IntTag;
        values.push_back(FromTagInt(data, offset, TagContext::ListElement));
        break;
      case NbtTag::LongTag:
        lastType = NbtTag::LongTag;
        values.push_back(FromTagLong(data, offset, TagContext::ListElement));
        break;
      case NbtTag::FloatTag:
        lastType = NbtTag::FloatTag;
        values.push_back(FromTagFloat(data, offset, TagContext::ListElement));
        break;
      case NbtTag::DoubleTag:
        lastType = NbtTag::DoubleTag;
        values.push_back(FromTagDouble(data, offset, TagContext::ListElement));
        break;
      case NbtTag::ByteArrayTag:
        lastType = NbtTag::ByteArrayTag;
        values.push_back(FromTagByteArray(data, offset, TagContext::ListElement));
        break;
      case NbtTag::StringTag:
        lastType = NbtTag::StringTag;
        values.push_back(FromTagString(data, offset, TagContext::ListElement));
        break;
      case NbtTag::ListTag:
        lastType = NbtTag::ListTag;
        values.push_back(FromTagList(data, offset, TagContext::ListElement, depth + 1, maxDepth));
        break;
      case NbtTag::CompoundTag:
        lastType = NbtTag::CompoundTag;
        values.push_back(
            FromTagCompound(data, offset, TagContext::ListElement, depth + 1, maxDepth));
        break;
      case NbtTag::IntArrayTag:
        lastType = NbtTag::IntArrayTag;
        values.push_back(FromTagIntArray(data, offset, TagContext::ListElement));
        break;
      case NbtTag::LongArrayTag:
        lastType = NbtTag::LongArrayTag;
        values.push_back(FromTagLongArray(data, offset, TagContext::ListElement));
        break;
      default:
        throw std::runtime_error(
            std::format("Undefined NBT tag: {}", std::to_underlying(listType)));
        break;
      }
    }
    return Nbt::makeList(std::move(name), std::move(values));
  }

  Nbt FromTagIntArray(std::span<const uint8_t> data, std::size_t& offset, nbt::TagContext context) {
    std::string name = readTagHeader(data, offset, NbtTag::IntArrayTag, context);

    const size_t size = static_cast<size_t>(endian::readBigEndian<uint32_t>(data, offset));

    requireBytes(data, offset, size);
    std::vector<int32_t> values;
    values.reserve(size);

    for (uint32_t i = 0; i < size; i++) {
      values.push_back(static_cast<int32_t>(endian::readBigEndian<uint32_t>(data, offset)));
    }

    return Nbt::makeIntArray(std::move(name), std::move(values));
  }

  Nbt FromTagLongArray(std::span<const uint8_t> data,
                       std::size_t& offset,
                       nbt::TagContext context) {
    std::string name = readTagHeader(data, offset, NbtTag::LongArrayTag, context);

    const size_t size = static_cast<size_t>(endian::readBigEndian<uint32_t>(data, offset));

    requireBytes(data, offset, size);
    std::vector<int64_t> values;
    values.reserve(size);

    for (uint32_t i = 0; i < size; i++) {
      values.push_back(static_cast<int64_t>(endian::readBigEndian<uint64_t>(data, offset)));
    }

    return Nbt::makeLongArray(std::move(name), std::move(values));
  }
  Nbt FromTagCompound(std::span<const std::uint8_t> data,
                      size_t& offset,
                      nbt::TagContext context,
                      std::size_t depth,
                      std::size_t maxDepth) {
    std::string name = readTagHeader(data, offset, NbtTag::CompoundTag, context);
    std::vector<Nbt> values;
    while (offset < data.size()) {
      if (depth > maxDepth) {
        throw std::runtime_error("amx Depth reached");
      }

      NbtTag nextTag = static_cast<NbtTag>(data[offset]);

      switch (nextTag) {
      case NbtTag::EndTag:
        offset++;
        return Nbt::makeCompound(std::move(name), std::move(values));
      case NbtTag::ByteTag:
        values.push_back(FromTagByte(data, offset));
        break;
      case NbtTag::ShortTag:
        values.push_back(FromTagShort(data, offset));
        break;
      case NbtTag::IntTag:
        values.push_back(FromTagInt(data, offset));
        break;
      case NbtTag::LongTag:
        values.push_back(FromTagLong(data, offset));
        break;
      case NbtTag::FloatTag:
        values.push_back(FromTagFloat(data, offset));
        break;
      case NbtTag::DoubleTag:
        values.push_back(FromTagDouble(data, offset));
        break;
      case NbtTag::ByteArrayTag:
        values.push_back(FromTagByteArray(data, offset));
        break;
      case NbtTag::StringTag:
        values.push_back(FromTagString(data, offset));
        break;
      case NbtTag::ListTag:
        values.push_back(FromTagList(data, offset, TagContext::Named, depth + 1, maxDepth));
        break;
      case NbtTag::CompoundTag:
        values.push_back(FromTagCompound(data, offset, TagContext::Named, depth + 1, maxDepth));
        break;
      case NbtTag::IntArrayTag:
        values.push_back(FromTagIntArray(data, offset));
        break;
      case NbtTag::LongArrayTag:
        values.push_back(FromTagLongArray(data, offset));
        break;
      default:
        throw std::runtime_error(std::format("Undefined NBT tag: {}", std::to_underlying(nextTag)));
        break;
      }
    }
    if (context == TagContext::NonCompoundRoot) {
      if (values.size() > 1) {
        throw std::runtime_error("when root is not compound only one tag i allowed");
      }
      Nbt temp = values.front();
      return temp;
    }
    throw std::runtime_error("missing end Tag");
  }

  Nbt NonCompoundRoot(std::span<const std::uint8_t> data,
                      size_t& offset,
                      nbt::TagContext context,
                      std::size_t depth,
                      std::size_t maxDepth) {
    NbtTag nextTag = static_cast<NbtTag>(data[offset]);
    std::string name = "";
    if (context != TagContext::NamelessRootAndNonCompoundRoot) {

      name = readTagHeader(data, offset, nextTag, context);
    }
    std::vector<Nbt> values;
    while (offset < data.size()) {
      if (depth > maxDepth) {
        throw std::runtime_error("amx Depth reached");
      }

      switch (nextTag) {
      case NbtTag::EndTag:
        offset++;
        return Nbt::makeCompound(std::move(name), std::move(values));
      case NbtTag::ByteTag:
        values.push_back(FromTagByte(data, offset, context));
        break;
      case NbtTag::ShortTag:
        values.push_back(FromTagShort(data, offset, context));
        break;
      case NbtTag::IntTag:
        values.push_back(FromTagInt(data, offset, context));
        break;
      case NbtTag::LongTag:
        values.push_back(FromTagLong(data, offset, context));
        break;
      case NbtTag::FloatTag:
        values.push_back(FromTagFloat(data, offset, context));
        break;
      case NbtTag::DoubleTag:
        values.push_back(FromTagDouble(data, offset, context));
        break;
      case NbtTag::ByteArrayTag:
        values.push_back(FromTagByteArray(data, offset, context));
        break;
      case NbtTag::StringTag:
        values.push_back(FromTagString(data, offset, context));
        break;
      case NbtTag::ListTag:
        values.push_back(FromTagList(data, offset, TagContext::Named, depth + 1, maxDepth));
        break;
      case NbtTag::CompoundTag:
        values.push_back(FromTagCompound(data, offset, TagContext::Named, depth + 1, maxDepth));
        break;
      case NbtTag::IntArrayTag:
        values.push_back(FromTagIntArray(data, offset, context));
        break;
      case NbtTag::LongArrayTag:
        values.push_back(FromTagLongArray(data, offset, context));
        break;
      default:
        throw std::runtime_error(std::format("Undefined NBT tag: {}", std::to_underlying(nextTag)));
        break;
      }
    }
    if (context == TagContext::NonCompoundRoot ||
        context == TagContext::NamelessRootAndNonCompoundRoot) {
      if (values.size() > 1) {
        throw std::runtime_error("when root is not compound only one tag i allowed");
      }
      Nbt temp = values.front();
      return temp;
    }
    throw std::runtime_error("missing end Tag");
  }

  namespace value {
    std::int8_t
    FromTagByte(std::span<const uint8_t>& data, std::size_t& offset, nbt::TagContext context) {
      return std::get<std::int8_t>(nbt::FromTagByte(data, offset).value());
    }

    std::int16_t
    FromTagShort(std::span<const std::uint8_t> data, std::size_t& offset, nbt::TagContext context) {
      return std::get<std::int8_t>(nbt::FromTagByte(data, offset).value());
    }

    std::int32_t
    FromTagInt(std::span<const std::uint8_t>& data, std::size_t& offset, nbt::TagContext context) {
      return std::get<std::int8_t>(nbt::FromTagByte(data, offset).value());
    }

    std::int64_t
    FromTagLong(std::span<const std::uint8_t>& data, std::size_t& offset, nbt::TagContext context) {
      return std::get<std::int8_t>(nbt::FromTagByte(data, offset).value());
    }

    float FromTagFloat(std::span<const std::uint8_t>& data,
                       std::size_t& offset,
                       nbt::TagContext context) {
      return std::get<std::int8_t>(nbt::FromTagByte(data, offset).value());
    }

    double FromTagDouble(std::span<const std::uint8_t>& data,
                         std::size_t& offset,
                         nbt::TagContext context) {
      return std::get<std::int8_t>(nbt::FromTagByte(data, offset).value());
    }

    std::vector<std::int8_t>
    FromTagByteArray(std::span<const uint8_t>& data, std::size_t& offset, nbt::TagContext context) {
      return std::get<std::vector<std::int8_t>>(nbt::FromTagByte(data, offset).value());
    }

    std::string FromTagString(std::span<const std::uint8_t>& data,
                              std::size_t& offset,
                              nbt::TagContext context) {
      return std::get<std::string>(nbt::FromTagByte(data, offset).value());
    }

    std::vector<castle::nbt::Nbt>
    FromTagList(std::span<const uint8_t>& data, std::size_t& offset, nbt::TagContext context) {
      return std::get<std::vector<Nbt>>(nbt::FromTagByte(data, offset).value());
    }

    std::vector<castle::nbt::Nbt>
    FromTagCompound(std::span<const std::uint8_t> data, size_t& offset, nbt::TagContext context) {
      return std::get<std::vector<Nbt>>(nbt::FromTagCompound(data, offset).value());
    }

    std::vector<int32_t>
    FromTagIntArray(std::span<const uint8_t> data, std::size_t& offset, nbt::TagContext context) {

      return std::get<std::vector<std::int32_t>>(nbt::FromTagByte(data, offset).value());
    }

    std::vector<int64_t>
    FromTagLongArray(std::span<const uint8_t>& data, std::size_t& offset, nbt::TagContext context) {

      return std::get<std::vector<std::int64_t>>(nbt::FromTagByte(data, offset, context).value());
    }

  } // namespace value
} // namespace castle::nbt
