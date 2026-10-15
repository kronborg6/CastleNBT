#include "endian.hpp"
#include <castle/nbt.hpp>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace castle::nbt {

  template <typename T> T toNbtLength(size_t length) {

    static_assert(std::is_same_v<T, uint16_t> || std::is_same_v<T, int32_t>,
                  "Unsupported NBT legth type");

    if constexpr (std::is_same_v<T, uint16_t>) {
      if (length > std::numeric_limits<uint16_t>::max()) {
        throw std::runtime_error("NBT string length is too large");
      }
    }

    if constexpr (std::is_same_v<T, int32_t>) {
      if (length > std::numeric_limits<int32_t>::max()) {
        throw std::runtime_error("NBT array length is too large");
      }
    }

    return static_cast<T>(length);
  }

  constexpr std::uint8_t tag(NbtTag value) noexcept {
    return static_cast<uint8_t>(value);
  }

  template <NbtTag Tag>
  constexpr void
  insertNameAndTag(std::string_view name, std::vector<uint8_t>& data, TagContext context) {
    if (context == TagContext::ListElement || Tag == NbtTag::EndTag)
      return;

    static_assert(Tag != NbtTag::EndTag, "EndTag does not have a name");

    const std::uint16_t rawLength = toNbtLength<uint16_t>(name.length());

    data.push_back(tag(Tag));

    endian::writeBigEndian<uint16_t>(rawLength, data);

    data.insert(data.end(), name.begin(), name.end());
  }

  void ToEndTag(std::vector<uint8_t>& data) {
    data.push_back(tag(NbtTag::EndTag));
  }

  void
  ToTagByte(std::string_view name, int8_t value, std::vector<uint8_t>& data, TagContext context) {

    insertNameAndTag<NbtTag::ByteTag>(name, data, context);

    data.push_back(static_cast<uint8_t>(value));
  }

  void ToTagShort(std::string_view name,
                  int16_t value,
                  std::vector<std::uint8_t>& data,
                  TagContext context) {

    insertNameAndTag<NbtTag::ShortTag>(name, data, context);

    const uint16_t rawValue = static_cast<uint16_t>(value);

    endian::writeBigEndian<uint16_t>(rawValue, data);
  }

  void ToTagInt(std::string_view name,
                int32_t value,
                std::vector<std::uint8_t>& data,
                TagContext context) {

    insertNameAndTag<NbtTag::IntTag>(name, data, context);

    const uint32_t rawValue = static_cast<uint32_t>(value);

    endian::writeBigEndian<uint32_t>(rawValue, data);
  }

  void ToTagLong(std::string_view name,
                 int64_t value,
                 std::vector<std::uint8_t>& data,
                 TagContext context) {

    insertNameAndTag<NbtTag::LongTag>(name, data, context);

    const uint64_t rawValue = static_cast<uint64_t>(value);

    endian::writeBigEndian<uint64_t>(rawValue, data);
  }

  void ToTagFloat(std::string_view name,
                  float value,
                  std::vector<std::uint8_t>& data,
                  TagContext context) {

    insertNameAndTag<NbtTag::FloatTag>(name, data, context);

    uint32_t rawFloat = endian::floatToUint32(value);

    endian::writeBigEndian<uint32_t>(rawFloat, data);
  }

  void ToTagDouble(std::string_view name,
                   double value,
                   std::vector<std::uint8_t>& data,
                   TagContext context) {

    insertNameAndTag<NbtTag::DoubleTag>(name, data, context);

    uint64_t rawFloat = endian::doubleToUint64(value);

    endian::writeBigEndian<uint64_t>(rawFloat, data);
  }

  void ToTagCompound(std::string_view name, std::vector<std::uint8_t>& data, TagContext context) {

    insertNameAndTag<NbtTag::CompoundTag>(name, data, context);
  }

  void ToTagByteArray(std::string_view name,
                      const std::vector<int8_t>& values,
                      std::vector<uint8_t>& data,
                      TagContext context) {

    insertNameAndTag<NbtTag::ByteArrayTag>(name, data, context);

    const int32_t size = toNbtLength<int32_t>(values.size());

    endian::writeBigEndian(static_cast<uint32_t>(size), data);

    for (int8_t value : values) {
      data.push_back(static_cast<uint8_t>(value));
    }
  }

  void ToTagString(std::string_view name,
                   const std::string& value,
                   std::vector<std::uint8_t>& data,
                   TagContext context) {

    insertNameAndTag<NbtTag::StringTag>(name, data, context);

    const uint16_t rawValueLength = toNbtLength<uint16_t>(value.length());

    endian::writeBigEndian<uint16_t>(rawValueLength, data);

    data.insert(data.end(), value.begin(), value.end());
  }

  // template <NbtTag Tag>
  void ToTagList(std::string_view name,
                 NbtTag valueType,
                 size_t listLength,
                 std::vector<std::uint8_t>& data,
                 TagContext context) {

    insertNameAndTag<NbtTag::ListTag>(name, data, context);

    const int32_t rawListLength = toNbtLength<int32_t>(listLength);

    if (listLength == 0) {
      data.push_back(tag(NbtTag::EndTag));
    } else {
      data.push_back(tag(valueType));
    }

    endian::writeBigEndian<uint32_t>(static_cast<uint32_t>(rawListLength), data);
  }

  void ToTagIntArray(std::string_view name,
                     const std::vector<int32_t>& values,
                     std::vector<std::uint8_t>& data,
                     TagContext context) {

    insertNameAndTag<NbtTag::IntArrayTag>(name, data, context);

    const int32_t size = toNbtLength<int32_t>(values.size());

    endian::writeBigEndian<uint32_t>(static_cast<uint32_t>(size), data);

    for (int32_t value : values) {
      const uint32_t rawItem = static_cast<uint32_t>(value);
      endian::writeBigEndian<uint32_t>(rawItem, data);
    }
  }

  void ToTagLongArray(std::string_view name,
                      const std::vector<int64_t>& values,
                      std::vector<std::uint8_t>& data,
                      TagContext context) {

    insertNameAndTag<NbtTag::LongArrayTag>(name, data, context);

    const int32_t size = toNbtLength<int32_t>(values.size());

    endian::writeBigEndian<uint32_t>(static_cast<uint32_t>(size), data);

    for (int64_t value : values) {
      const uint64_t rawItem = static_cast<uint64_t>(value);
      endian::writeBigEndian<uint64_t>(rawItem, data);
    }
  }

} // namespace castle::nbt
