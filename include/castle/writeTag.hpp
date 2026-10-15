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

  void ToTagByte(const std::string& name,
                 int8_t value,
                 std::vector<uint8_t>& data,
                 TagContext context = TagContext::Named);

  void ToTagShort(const std::string& name,
                  int16_t value,
                  std::vector<std::uint8_t>& data,
                  TagContext context = TagContext::Named);

  void ToTagInt(const std::string& name,
                int32_t value,
                std::vector<std::uint8_t>& data,
                TagContext context = TagContext::Named);

  void ToTagLong(const std::string& name,
                 int64_t value,
                 std::vector<std::uint8_t>& data,
                 TagContext context = TagContext::Named);

  void ToTagFloat(const std::string& name,
                  float value,
                  std::vector<std::uint8_t>& data,
                  TagContext context = TagContext::Named);

  void ToTagDouble(const std::string& name,
                   double value,
                   std::vector<std::uint8_t>& data,
                   TagContext context = TagContext::Named);

  void ToTagByteArray(const std::string& name,
                      const std::vector<int8_t>& values,
                      std::vector<uint8_t>& data,
                      TagContext context = TagContext::Named);

  void ToTagString(const std::string& name,
                   const std::string& value,
                   std::vector<std::uint8_t>& data,
                   TagContext context = TagContext::Named);

  void ToTagList(const std::string& name,
                 NbtTag valueType,
                 size_t listLength,
                 std::vector<uint8_t>& data,
                 TagContext context = TagContext::Named);

  void ToTagCompound(const std::string& name,
                     std::vector<std::uint8_t>& data,
                     TagContext context = TagContext::Named);

  void ToTagIntArray(const std::string& name,
                     const std::vector<int32_t>& value,
                     std::vector<uint8_t>& data,
                     TagContext context = TagContext::Named);

  void ToTagLongArray(const std::string& name,
                      const std::vector<int64_t>& value,
                      std::vector<uint8_t>& data,
                      TagContext context = TagContext::Named);

} // namespace castle::nbt
