#include "endian.hpp"
#include <castle/nbt.hpp>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
#include <string>
#include <vector>

#include <string>
#include <vector>

namespace castle::nbt {

  // enum class NbtTag : std::uint8_t;

  void ToEndTag(std::vector<uint8_t>& data);

  Nbt FromTagByte(std::span<const std::uint8_t> data, std::size_t& offset) {
    NbtTag tag = static_cast<NbtTag>(data[offset++]);
    uint16_t nameLength = endian::readBigEndian<uint16_t>(data, offset);

    std::string name(data.begin() + offset, data.begin() + offset + nameLength);

    offset += nameLength;

    std::int8_t value = static_cast<std::int8_t>(data[offset++]);

    return Nbt::makeByte(name, value);
  }

  inline Nbt FromTagShort(const std::vector<std::uint8_t>& data, std::size_t& offset) {
  }

  inline Nbt FromTagInt(const std::vector<std::uint8_t>& data, std::size_t& offset) {
  }

  inline Nbt FromTagLong(const std::vector<std::uint8_t>& data, std::size_t& offset) {
  }

  inline Nbt FromTagFloat(const std::vector<std::uint8_t>& data, std::size_t& offset) {
  }

  inline Nbt FromTagDouble(const std::vector<std::uint8_t>& data, std::size_t& offset) {
  }

  inline Nbt FromTagByteArray(const std::vector<uint8_t>& data, std::size_t& offset) {
  }

  inline Nbt FromTagString(const std::vector<std::uint8_t>& data, std::size_t& offset) {
  }

  inline Nbt FromTagList(const std::vector<uint8_t>& data, std::size_t& offset) {
  }

  inline Nbt FromTagCompound(const std::vector<std::uint8_t>& data, size_t& offset) {
  }

  inline Nbt FromTagIntArray(const std::vector<uint8_t>& data, std::size_t& offset) {
  }

  inline Nbt FromTagLongArray(const std::vector<uint8_t>& data, std::size_t& offset) {
  }

  namespace value {
    std::int8_t FromTagByte(const std::vector<uint8_t>& data, std::size_t& offset) {
      return std::get<std::int8_t>(nbt::FromTagByte(data, offset).value());
    }

    inline std::int16_t FromTagShort(const std::vector<std::uint8_t>& data, std::size_t& offset) {
      return std::get<std::int8_t>(nbt::FromTagByte(data, offset).value());
    }

    inline std::int32_t FromTagInt(const std::vector<std::uint8_t>& data, std::size_t& offset) {
      return std::get<std::int8_t>(nbt::FromTagByte(data, offset).value());
    }

    inline std::int64_t FromTagLong(const std::vector<std::uint8_t>& data, std::size_t& offset) {
      return std::get<std::int8_t>(nbt::FromTagByte(data, offset).value());
    }

    inline float FromTagFloat(const std::vector<std::uint8_t>& data, std::size_t& offset) {
      return std::get<std::int8_t>(nbt::FromTagByte(data, offset).value());
    }

    inline double FromTagDouble(const std::vector<std::uint8_t>& data, std::size_t& offset) {
      return std::get<std::int8_t>(nbt::FromTagByte(data, offset).value());
    }

    inline std::vector<std::int8_t> FromTagByteArray(const std::vector<uint8_t>& data,
                                                     std::size_t& offset) {
      return std::get<std::vector<std::int8_t>>(nbt::FromTagByte(data, offset).value());
    }

    inline std::string FromTagString(const std::vector<std::uint8_t>& data, std::size_t& offset) {
      return std::get<std::string>(nbt::FromTagByte(data, offset).value());
    }

    inline std::vector<castle::nbt::NbtValue> FromTagList(const std::vector<uint8_t>& data,
                                                          std::size_t& offset) {
      // return std::get<std::vector<NbtValue>>(nbt::FromTagByte(data, offset).value());
    }

    inline Nbt FromTagCompound(const std::vector<std::uint8_t>& data, size_t& offset) {
      return FromTagCompound(data, offset);
    }

    inline std::vector<int32_t> FromTagIntArray(const std::vector<uint8_t>& data,
                                                std::size_t& offset) {

      return std::get<std::vector<std::int32_t>>(nbt::FromTagByte(data, offset).value());
    }

    inline std::vector<int64_t> FromTagLongArray(const std::vector<uint8_t>& data,
                                                 std::size_t& offset) {

      return std::get<std::vector<std::int64_t>>(nbt::FromTagByte(data, offset).value());
    }

  } // namespace value
} // namespace castle::nbt
