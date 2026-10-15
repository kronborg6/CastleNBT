#pragma once

#include <castle/writeTag.hpp>
#include <cstdint>
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
      std::optional<std::string> name;

      void add(Nbt child);
      void append(Nbt child);

    private:
      castle::nbt::NbtTag _tag;
      NbtValue _value;
  };

  constexpr std::uint8_t tag(NbtTag value) noexcept;

  template <NbtTag Tag>
  constexpr void
  insertNameAndTag(const std::string& name, std::vector<uint8_t>& data, TagContext context);

  void
  writeNbt(const Nbt& node, std::vector<uint8_t>& data, TagContext context = TagContext::Named);

  Nbt makeCompound(std::string name);
  Nbt makeString(std::string name, std::string value);

} // namespace castle::nbt
