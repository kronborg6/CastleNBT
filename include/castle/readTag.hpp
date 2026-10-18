#pragma once

#include "nbt.hpp"
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <vector>

#include <string>
#include <vector>

namespace castle::nbt {

  // enum class NbtTag : std::uint8_t;

  std::string readTagHeader(std::span<const std::uint8_t> data, std::size_t& offset, NbtTag expted);

  void ToEndTag(std::vector<uint8_t>& data);

  Nbt FromTagByte(std::span<const std::uint8_t> data, std::size_t& offset);

  Nbt FromTagShort(std::span<const std::uint8_t> data, std::size_t& offset);

  Nbt FromTagInt(std::span<const std::uint8_t> data, std::size_t& offset);

  Nbt FromTagLong(std::span<const std::uint8_t> data, std::size_t& offset);

  Nbt FromTagFloat(std::span<const std::uint8_t> data, std::size_t& offset);

  Nbt FromTagDouble(std::span<const std::uint8_t> data, std::size_t& offset);

  Nbt FromTagByteArray(std::span<const uint8_t> data, std::size_t& offset);

  Nbt FromTagString(std::span<const std::uint8_t> data, std::size_t& offset);

  Nbt FromTagList(std::span<const uint8_t> data, std::size_t& offset);

  Nbt FromTagCompound(std::span<const std::uint8_t> data, size_t& offset);

  Nbt FromTagIntArray(std::span<const uint8_t> data, std::size_t& offset);

  Nbt FromTagLongArray(std::span<const uint8_t> data, std::size_t& offset);

  namespace value {
    std::int8_t FromTagByte(std::span<const uint8_t> data, std::size_t& offset);

    std::int16_t FromTagShort(std::span<const std::uint8_t> data, std::size_t& offset);

    std::int32_t FromTagInt(std::span<const std::uint8_t> data, std::size_t& offset);

    std::int64_t FromTagLong(std::span<const std::uint8_t> data, std::size_t& offset);

    float FromTagFloat(std::span<const std::uint8_t> data, std::size_t& offset);

    double FromTagDouble(std::span<const std::uint8_t> data, std::size_t& offset);

    std::vector<std::int8_t> FromTagByteArray(const std::vector<uint8_t> data, std::size_t& offset);

    std::string FromTagString(std::span<const std::uint8_t>& data, std::size_t& offset);

    std::vector<castle::nbt::NbtValue> FromTagList(std::span<const uint8_t> data,
                                                   std::size_t& offset);

    Nbt FromTagCompound(std::span<const std::uint8_t>& data, size_t offset);

    std::vector<int32_t> FromTagIntArray(std::span<const uint8_t> data, std::size_t& offset);

    std::vector<int64_t> FromTagLongArray(std::span<const uint8_t> data, std::size_t& offset);
  } // namespace value
} // namespace castle::nbt
