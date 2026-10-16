#include "castle/nbt.hpp"
#include "internal.hpp"
#include <variant>
namespace castle::nbt {

  NbtTag tagFromVarible(const NbtValue& value) {
    return std::visit(
        [&](const auto& v) -> NbtTag {
          using T = std::decay_t<decltype(v)>;
          if constexpr (std::is_same_v<T, std::monostate>) {
            throw std::runtime_error("can't create EndNBT");
          } else if constexpr (std::is_same_v<T, int8_t>) {
            return NbtTag::ByteTag;
          } else if constexpr (std::is_same_v<T, int16_t>) {
            return NbtTag::IntTag;
          } else if constexpr (std::is_same_v<T, int32_t>) {
            return NbtTag::DoubleTag;
          } else if constexpr (std::is_same_v<T, int64_t>) {
            return NbtTag::LongTag;
          } else if constexpr (std::is_same_v<T, float>) {
            return NbtTag::FloatTag;
          } else if constexpr (std::is_same_v<T, double>) {
            return NbtTag::DoubleTag;
          } else if constexpr (std::is_same_v<T, std::string>) {
            return NbtTag::StringTag;
          } else if constexpr (std::is_same_v<T, std::vector<int8_t>>) {
            return NbtTag::ByteArrayTag;
          } else if constexpr (std::is_same_v<T, std::vector<int32_t>>) {
            return NbtTag::IntArrayTag;
          } else if constexpr (std::is_same_v<T, std::vector<int64_t>>) {
            return NbtTag::LongArrayTag;
          } else {
            throw std::runtime_error(
                "Create compounds and lists with makeCompound() or makeList()");
          }
        },
        value);
  }
  VariableType fromVarible(const NbtValue& value) {
    return std::visit(
        [&](const auto& v) -> VariableType {
          using T = std::decay_t<decltype(v)>;
          if constexpr (std::is_same_v<T, std::monostate>) {
            throw std::runtime_error("can't create EndNBT");
          } else if constexpr (std::is_same_v<T, int8_t>) {
            return VariableType::Byte;
          } else if constexpr (std::is_same_v<T, int16_t>) {
            return VariableType::Short;
          } else if constexpr (std::is_same_v<T, int32_t>) {
            return VariableType::Int;
          } else if constexpr (std::is_same_v<T, int64_t>) {
            return VariableType::Long;
          } else if constexpr (std::is_same_v<T, float>) {
            return VariableType::Float;
          } else if constexpr (std::is_same_v<T, double>) {
            return VariableType::Double;
          } else if constexpr (std::is_same_v<T, std::string>) {
            return VariableType::String;
          } else if constexpr (std::is_same_v<T, std::vector<int8_t>>) {
            return VariableType::ByteArray;
          } else if constexpr (std::is_same_v<T, std::vector<int32_t>>) {
            return VariableType::IntArray;
          } else if constexpr (std::is_same_v<T, std::vector<int64_t>>) {
            return VariableType::LongArray;
          } else {
            throw std::runtime_error(
                "Create compounds and lists with makeCompound() or makeList()");
          }
        },
        value);
  }
} // namespace castle::nbt
