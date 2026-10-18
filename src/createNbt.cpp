#include <algorithm>
#include <castle/nbt.hpp>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

namespace castle::nbt {

Nbt Nbt::makeCompound(std::string name, std::vector<Nbt> value) {
  return Nbt(name, NbtTag::CompoundTag, value);
}
Nbt Nbt::makeString(std::string name, std::string value) {
  return Nbt(name, NbtTag::StringTag, value);
}
Nbt Nbt::makeByte(std::string name, std::int8_t value) {
  return Nbt(name, NbtTag::ByteTag, value);
}
Nbt Nbt::makeShort(std::string name, std::int16_t value) {
  return Nbt(name, NbtTag::ShortTag, value);
}
Nbt Nbt::makeInt(std::string name, std::int32_t value) {
  return Nbt(name, NbtTag::IntTag, value);
}
Nbt Nbt::makeLong(std::string name, std::int64_t value) {
  return Nbt(name, NbtTag::LongTag, value);
}
Nbt Nbt::makeFloat(std::string name, float value) {
  return Nbt(name, NbtTag::FloatTag, value);
}
Nbt Nbt::makeDouble(std::string name, double value) {
  return Nbt(name, NbtTag::DoubleTag, value);
}
Nbt Nbt::makeByteArray(std::string name, std::vector<std::int8_t> value) {
  return Nbt(name, NbtTag::ByteArrayTag, value);
}
Nbt Nbt::makeList(std::string name, NbtValue value) {
  return Nbt(name, NbtTag::ListTag, value);
}
Nbt Nbt::makeIntArray(std::string name, std::vector<std::int32_t> value) {
  return Nbt(name, NbtTag::IntArrayTag, value);
}
Nbt Nbt::makeLongArray(std::string name, std::vector<std::int64_t> value) {
  return Nbt(name, NbtTag::LongArrayTag, value);
}

Nbt makeNbt(const NbtValue &value) { return makeNbt(std::string{}, value); }
Nbt makeNbt(std::string name, const NbtValue &value) {
  return std::visit(
      [&name](const auto &v) -> Nbt {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
          throw std::runtime_error("cant create endNBT");
        } else if constexpr (std::is_same_v<T, int8_t>) {
          return Nbt::makeByte(std::move(name), v);
        } else if constexpr (std::is_same_v<T, int16_t>) {
          return Nbt::makeShort(std::move(name), v);
        } else if constexpr (std::is_same_v<T, int32_t>) {
          return Nbt::makeInt(std::move(name), v);
        } else if constexpr (std::is_same_v<T, int64_t>) {
          return Nbt::makeLong(std::move(name), v);
        } else if constexpr (std::is_same_v<T, float>) {
          return Nbt::makeFloat(std::move(name), v);
        } else if constexpr (std::is_same_v<T, double>) {
          return Nbt::makeDouble(std::move(name), v);
        } else if constexpr (std::is_same_v<T, std::string>) {
          return Nbt::makeString(std::move(name), v);
        } else if constexpr (std::is_same_v<T, std::vector<int8_t>>) {
          return Nbt::makeByteArray(std::move(name), v);
        } else if constexpr (std::is_same_v<T, std::vector<int32_t>>) {
          return Nbt::makeIntArray(std::move(name), v);
        } else if constexpr (std::is_same_v<T, std::vector<int64_t>>) {
          return Nbt::makeLongArray(std::move(name), v);
        } else {
          throw std::runtime_error(
              "make compound and list with makeCompound() or makeList()");
        }
      },
      value);
}
} // namespace castle::nbt
