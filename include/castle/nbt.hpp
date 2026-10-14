#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <optional>
#include <string>
#include <variant>
#include <vector>
namespace castle::nbt {

  template <typename T> T toNbtLength(size_t length);

  enum class NbtTag : std::uint8_t {
    EndTag = 0x00,
    ByteTag = 0x01,
    ShortTag = 0x02,
    IntTag = 0x03,
    LongTag = 0x04,
    FloatTag = 0x05,
    DoubleTag = 0x06,
    ByteArrayTag = 0x07,
    StringTag = 0x08,
    ListTag = 0x09,
    CompoundTag = 0x0A,
    IntArrayTag = 0x0B,
    LongArrayTag = 0x0C,
  };
  enum class TagContext {
    Named,
    ListElement,
  };

  struct Nbt;

  using NbtValue = std::variant<std::monostate,
                                int8_t,
                                int16_t,
                                int32_t,
                                int64_t,
                                float,
                                double,
                                std::string,
                                std::vector<int8_t>,
                                std::vector<Nbt>,
                                std::vector<int32_t>,
                                std::vector<int64_t>>;

  struct Nbt {
      castle::nbt::NbtTag tag;
      castle::nbt::TagContext context;
      std::optional<std::string> name;
      NbtValue value;
  };

  constexpr std::uint8_t tag(NbtTag value) noexcept;

  template <NbtTag Tag>
  constexpr void
  insertNameAndTag(const std::string& name, std::vector<uint8_t>& data, TagContext context);

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

  // void ToTagList(const std::string& name,
  //                NbtTag valueType,
  //                std::vector<uint8_t>& data,
  //                TagContext context = TagContext::Named);
  // template <NbtTag Tag>
  // void ToTagList(const std::string& name,
  //                size_t listLength,
  //                std::vector<uint8_t>& data,
  //                TagContext context = TagContext::Named);

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

  void writeNbt(const Nbt& node, std::vector<uint8_t>& data);
} // namespace castle::nbt
