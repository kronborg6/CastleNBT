#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include <string>
#include <vector>

namespace castle::nbt {

  enum class NbtTag : std::uint8_t;

  void ToEndTag(std::vector<uint8_t>& data);

  template <typename T> T FromTagByte(std::vector<uint8_t>& data);

  template <typename T> T FromTagShort(std::vector<std::uint8_t>& data);

  template <typename T> T FromTagInt(std::vector<std::uint8_t>& data);

  template <typename T> T FromTagLong(std::vector<std::uint8_t>& data);

  template <typename T> T FromTagFloat(std::vector<std::uint8_t>& data);

  template <typename T> T FromTagDouble(std::vector<std::uint8_t>& data);

  template <typename T> T ToTagByteArray(std::vector<uint8_t>& data);

  template <typename T> T FromTagString(std::vector<std::uint8_t>& data);

  template <typename T> T FromTagList(std::vector<uint8_t>& data);

  template <typename T> T FromTagCompound(const std::string& name, std::vector<std::uint8_t>& data);

  template <typename T> T FromTagIntArray(std::vector<uint8_t>& data);

  template <typename T> T FromTagLongArray(std::vector<uint8_t>& data);

} // namespace castle::nbt
