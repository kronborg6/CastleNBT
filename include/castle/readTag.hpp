#pragma once
#include "nbt.hpp"
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include <string>
#include <vector>

namespace castle::nbt {

  enum class NbtTag : std::uint8_t;

  void ToEndTag(std::vector<uint8_t>& data);
  //
  // template <typename T, typename Raw>
  // concept RawOrNbt = std::same_as<T, Raw> || std::same_as<T, Nbt>;
  //
  inline Nbt FromTagByte(const std::vector<uint8_t>& data, std::size_t& offset);

  inline Nbt FromTagShort(const std::vector<std::uint8_t>& data, std::size_t& offset);

  inline Nbt FromTagInt(const std::vector<std::uint8_t>& data, std::size_t& offset);

  inline Nbt FromTagLong(const std::vector<std::uint8_t>& data, std::size_t& offset);

  inline Nbt FromTagFloat(const std::vector<std::uint8_t>& data, std::size_t& offset);

  inline Nbt FromTagDouble(const std::vector<std::uint8_t>& data, std::size_t& offset);

  inline Nbt FromTagByteArray(const std::vector<uint8_t>& data, std::size_t& offset);

  inline Nbt FromTagString(const std::vector<std::uint8_t>& data, std::size_t& offset);

  inline Nbt FromTagList(const std::vector<uint8_t>& data, std::size_t& offset);

  inline Nbt FromTagCompound(const std::vector<std::uint8_t>& data, size_t& offset);

  inline Nbt FromTagIntArray(const std::vector<uint8_t>& data, std::size_t& offset);

  inline Nbt FromTagLongArray(const std::vector<uint8_t>& data, std::size_t& offset);

  namespace value {

    std::int8_t FromTagByte(const std::vector<uint8_t>& data, std::size_t& offset);

    std::int16_t FromTagShort(const std::vector<std::uint8_t>& data, std::size_t& offset);

    std::int32_t FromTagInt(const std::vector<std::uint8_t>& data, std::size_t& offset);

    template <typename T> T FromTagLong(const std::vector<std::uint8_t>& data, std::size_t& offset);

    template <typename T>
    T FromTagFloat(const std::vector<std::uint8_t>& data, std::size_t& offset);

    template <typename T>
    T FromTagDouble(const std::vector<std::uint8_t>& data, std::size_t& offset);

    template <typename T> T FromTagByteArray(const std::vector<uint8_t>& data, std::size_t& offset);

    template <typename T>
    T FromTagString(const std::vector<std::uint8_t>& data, std::size_t& offset);

    template <typename T> T FromTagList(const std::vector<uint8_t>& data, std::size_t& offset);

    template <typename T> T FromTagCompound(const std::vector<std::uint8_t>& data, size_t& offset);

    template <typename T> T FromTagIntArray(const std::vector<uint8_t>& data, std::size_t& offset);

    template <typename T> T FromTagLongArray(const std::vector<uint8_t>& data, std::size_t& offset);

  } // namespace value
} // namespace castle::nbt
