#pragma once

#include <algorithm>
#include <castle/writeTag.hpp>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
namespace castle::nbt {

  template <typename T> T toNbtLength(size_t length);

  enum class NbtTag : std::uint8_t {
    EndTag = 0x00, // noted used by user get set by writeNbt (should not be add self only in raw
                   // function)
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

  class Nbt {
    public:
      static Nbt makeByte(std::string name, std::int8_t value);
      static Nbt makeShort(std::string name, std::int16_t value);
      static Nbt makeInt(std::string name, std::int32_t value);
      static Nbt makeLong(std::string name, std::int64_t value);
      static Nbt makeFloat(std::string name, float value);
      static Nbt makeDouble(std::string name, double value);
      static Nbt makeByteArray(std::string name, std::vector<std::int8_t> value = {});
      static Nbt makeString(std::string name, std::string value = {});
      static Nbt makeList(std::string name, std::vector<Nbt> value = {});
      static Nbt makeCompound(std::string name, std::vector<Nbt> value = {});
      static Nbt makeIntArray(std::string name, std::vector<std::int32_t> value = {});
      static Nbt makeLongArray(std::string name, std::vector<std::int64_t> value = {});

      void add(Nbt child);
      void append(Nbt child);

      NbtTag tag() const {
        return _tag;
      }

      std::string_view name() const {
        return _name;
      }

      void rename(std::string newName) {
        _name = std::move(newName);
      }
      void clearName() {
        _name.clear();
      }

      const NbtValue& value() const {
        return _value;
      }

    private:
      Nbt(std::string name, castle::nbt::NbtTag tag, NbtValue value);

      std::string _name = "";
      castle::nbt::NbtTag _tag;
      NbtValue _value;
  };

  constexpr std::uint8_t tag(NbtTag value) noexcept;

  template <NbtTag Tag>
  constexpr void
  insertNameAndTag(std::string_view name, std::vector<uint8_t>& data, TagContext context);

  void
  writeNbt(const Nbt& node, std::vector<uint8_t>& data, TagContext context = TagContext::Named);

} // namespace castle::nbt
