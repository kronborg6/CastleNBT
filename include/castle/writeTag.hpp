#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include <string>
#include <vector>

namespace castle::nbt {

  enum class NbtTag : std::uint8_t;

  enum class TagContext {
    Named,
    ListElement,
  };

  void ToEndTag(std::vector<uint8_t>& data);

  void ToTagByte(std::string_view name,
                 int8_t value,
                 std::vector<uint8_t>& data,
                 TagContext context = TagContext::Named);

  void ToTagShort(std::string_view name,
                  int16_t value,
                  std::vector<std::uint8_t>& data,
                  TagContext context = TagContext::Named);

  void ToTagInt(std::string_view name,
                int32_t value,
                std::vector<std::uint8_t>& data,
                TagContext context = TagContext::Named);

  void ToTagLong(std::string_view name,
                 int64_t value,
                 std::vector<std::uint8_t>& data,
                 TagContext context = TagContext::Named);

  void ToTagFloat(std::string_view name,
                  float value,
                  std::vector<std::uint8_t>& data,
                  TagContext context = TagContext::Named);

  void ToTagDouble(std::string_view name,
                   double value,
                   std::vector<std::uint8_t>& data,
                   TagContext context = TagContext::Named);

  void ToTagByteArray(std::string_view name,
                      const std::vector<int8_t>& values,
                      std::vector<uint8_t>& data,
                      TagContext context = TagContext::Named);

  void ToTagString(std::string_view name,
                   const std::string& value,
                   std::vector<std::uint8_t>& data,
                   TagContext context = TagContext::Named);

  void ToTagList(std::string_view name,
                 NbtTag valueType,
                 size_t listLength,
                 std::vector<uint8_t>& data,
                 TagContext context = TagContext::Named);

  void ToTagCompound(std::string_view name,
                     std::vector<std::uint8_t>& data,
                     TagContext context = TagContext::Named);

  void ToTagIntArray(std::string_view name,
                     const std::vector<int32_t>& value,
                     std::vector<uint8_t>& data,
                     TagContext context = TagContext::Named);

  void ToTagLongArray(std::string_view name,
                      const std::vector<int64_t>& value,
                      std::vector<uint8_t>& data,
                      TagContext context = TagContext::Named);

} // namespace castle::nbt
