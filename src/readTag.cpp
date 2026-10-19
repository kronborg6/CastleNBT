#include "endian.hpp"
#include <algorithm>
#include <bit>
#include <castle/nbt.hpp>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <format>
#include <span>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <utility>
#include <vector>

#include <string>
#include <vector>

namespace castle::nbt {

  // enum class NbtTag : std::uint8_t;

  std::string
  readTagHeader(std::span<const std::uint8_t> data, std::size_t& offset, NbtTag exepted) {
    if (offset >= data.size()) {
      throw std::runtime_error("unexpected end of data");
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

  Nbt FromTagByte(std::span<const std::uint8_t> data, std::size_t& offset) {
    std::string name = readTagHeader(data, offset, NbtTag::ByteTag);

    std::int8_t value = static_cast<std::int8_t>(data[offset++]);

    return Nbt::makeByte(std::move(name), value);
  }

  Nbt FromTagShort(std::span<const std::uint8_t> data, std::size_t& offset) {
    std::string name = readTagHeader(data, offset, NbtTag::ShortTag);

    int16_t value = static_cast<int16_t>(endian::readBigEndian<uint16_t>(data, offset));

    return Nbt::makeShort(std::move(name), value);
  }

  Nbt FromTagInt(std::span<const std::uint8_t> data, std::size_t& offset) {
    std::string name = readTagHeader(data, offset, NbtTag::IntTag);

    int32_t value = static_cast<int32_t>(endian::readBigEndian<uint32_t>(data, offset));

    return Nbt::makeInt(std::move(name), value);
  }

  Nbt FromTagLong(std::span<const std::uint8_t> data, std::size_t& offset) {
    std::string name = readTagHeader(data, offset, NbtTag::LongTag);

    int64_t value = static_cast<int64_t>(endian::readBigEndian<uint64_t>(data, offset));

    return Nbt::makeLong(std::move(name), value);
  }

  Nbt FromTagFloat(std::span<const std::uint8_t> data, std::size_t& offset) {
    std::string name = readTagHeader(data, offset, NbtTag::FloatTag);

    uint32_t rawFloat = endian::readBigEndian<uint32_t>(data, offset);

    float value = endian::uint32ToFloat(rawFloat);

    return Nbt::makeFloat(std::move(name), value);
  }

  Nbt FromTagDouble(std::span<const std::uint8_t> data, std::size_t& offset) {
    std::string name = readTagHeader(data, offset, NbtTag::DoubleTag);

    uint64_t rawDouble = endian::readBigEndian<uint64_t>(data, offset);

    double value = endian::uint64ToDouble(rawDouble);

    return Nbt::makeDouble(std::move(name), value);
  }

  Nbt FromTagByteArray(std::span<const uint8_t> data, std::size_t& offset) {
    std::string name = readTagHeader(data, offset, NbtTag::ByteArrayTag);

    const size_t size = static_cast<size_t>(endian::readBigEndian<uint32_t>(data, offset));

    std::vector<int8_t> values(data.begin() + offset, data.begin() + size + offset);
    offset += size;

    return Nbt::makeByteArray(std::move(name), std::move(values));
  }

  Nbt FromTagString(std::span<const std::uint8_t> data, std::size_t& offset) {
    std::string name = readTagHeader(data, offset, NbtTag::StringTag);

    uint16_t valueLengh = endian::readBigEndian<uint16_t>(data, offset);

    std::string value(data.begin() + offset, data.begin() + valueLengh + offset);
    offset += valueLengh;

    return Nbt::makeString(std::move(name), std::move(value));
  }

  Nbt FromTagList(std::span<const uint8_t> data, std::size_t& offset) {
    std::string name = readTagHeader(data, offset, NbtTag::ListTag);

    NbtTag listType = static_cast<NbtTag>(data[offset++]);

    std::size_t listLength =
        static_cast<std::size_t>(endian::readBigEndian<uint32_t>(data, offset));

    switch (listType) {
    case NbtTag::IntTag: {

      std::vector<int32_t> values;
      break;
    }
    defualt:
      break;
    }
    return Nbt::makeList(name);
  }

  Nbt FromTagIntArray(std::span<const uint8_t> data, std::size_t& offset) {
    std::string name = readTagHeader(data, offset, NbtTag::IntArrayTag);

    const size_t size = static_cast<size_t>(endian::readBigEndian<uint32_t>(data, offset));

    std::vector<int32_t> values;
    values.reserve(size);

    for (uint32_t i = 0; i < size; i++) {
      values.push_back(static_cast<int32_t>(endian::readBigEndian<uint32_t>(data, offset)));
    }

    return Nbt::makeIntArray(std::move(name), std::move(values));
  }

  Nbt FromTagLongArray(std::span<const uint8_t> data, std::size_t& offset) {
    std::string name = readTagHeader(data, offset, NbtTag::LongArrayTag);

    const size_t size = static_cast<size_t>(endian::readBigEndian<uint32_t>(data, offset));

    std::vector<int64_t> values;
    values.reserve(size);

    for (uint32_t i = 0; i < size; i++) {
      values.push_back(static_cast<int64_t>(endian::readBigEndian<uint64_t>(data, offset)));
    }

    return Nbt::makeLongArray(std::move(name), std::move(values));
  }
  Nbt FromTagCompound(std::span<const std::uint8_t> data, size_t& offset) {
    std::string name = readTagHeader(data, offset, NbtTag::CompoundTag);

    std::vector<Nbt> values;
    while (offset < data.size()) {
      NbtTag nextTag = static_cast<NbtTag>(data[offset]);
      switch (nextTag) {
      case NbtTag::EndTag:
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
        values.push_back(FromTagDouble(data, offset));
        break;
      case NbtTag::StringTag:
        values.push_back(FromTagString(data, offset));
        break;
      case NbtTag::ListTag:
        values.push_back(FromTagList(data, offset));
        break;
      case NbtTag::CompoundTag:
        values.push_back(FromTagCompound(data, offset));
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
    throw std::runtime_error("missing end Tag");
  }

  namespace value {
    std::int8_t FromTagByte(std::span<const uint8_t>& data, std::size_t& offset) {
      return std::get<std::int8_t>(nbt::FromTagByte(data, offset).value());
    }

    std::int16_t FromTagShort(std::span<const std::uint8_t> data, std::size_t& offset) {
      return std::get<std::int8_t>(nbt::FromTagByte(data, offset).value());
    }

    std::int32_t FromTagInt(std::span<const std::uint8_t>& data, std::size_t& offset) {
      return std::get<std::int8_t>(nbt::FromTagByte(data, offset).value());
    }

    std::int64_t FromTagLong(std::span<const std::uint8_t>& data, std::size_t& offset) {
      return std::get<std::int8_t>(nbt::FromTagByte(data, offset).value());
    }

    float FromTagFloat(std::span<const std::uint8_t>& data, std::size_t& offset) {
      return std::get<std::int8_t>(nbt::FromTagByte(data, offset).value());
    }

    double FromTagDouble(std::span<const std::uint8_t>& data, std::size_t& offset) {
      return std::get<std::int8_t>(nbt::FromTagByte(data, offset).value());
    }

    std::vector<std::int8_t> FromTagByteArray(std::span<const uint8_t>& data, std::size_t& offset) {
      return std::get<std::vector<std::int8_t>>(nbt::FromTagByte(data, offset).value());
    }

    std::string FromTagString(std::span<const std::uint8_t>& data, std::size_t& offset) {
      return std::get<std::string>(nbt::FromTagByte(data, offset).value());
    }

    std::vector<castle::nbt::NbtValue> FromTagList(std::span<const uint8_t>& data,
                                                   std::size_t& offset) {
      // return std::get<std::vector<NbtValue>>(nbt::FromTagByte(data,
      // offset).value());
    }

    Nbt FromTagCompound(std::span<const std::uint8_t>& data, size_t& offset) {
      return FromTagCompound(data, offset);
    }

    std::vector<int32_t> FromTagIntArray(std::span<const uint8_t> data, std::size_t& offset) {

      return std::get<std::vector<std::int32_t>>(nbt::FromTagByte(data, offset).value());
    }

    std::vector<int64_t> FromTagLongArray(std::span<const uint8_t>& data, std::size_t& offset) {

      return std::get<std::vector<std::int64_t>>(nbt::FromTagByte(data, offset).value());
    }

  } // namespace value
} // namespace castle::nbt
