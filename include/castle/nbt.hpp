#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace castle::nbt {

  // struct NbtValue;

  template <typename T> T toNbtLength(size_t length);

  enum class NbtTag : std::uint8_t {
    EndTag = 0x00, // noted used by user get set by writeNbt (should not be add
                   // self only in raw function)
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
  class Nbt;

  template <NbtTag Tag> struct NbtValueOf;
  template <> struct NbtValueOf<NbtTag::ByteTag> {
      using type = int8_t;
  };
  template <> struct NbtValueOf<NbtTag::ShortTag> {
      using type = int16_t;
  };
  template <> struct NbtValueOf<NbtTag::IntTag> {
      using type = int32_t;
  };
  template <> struct NbtValueOf<NbtTag::LongTag> {
      using type = int64_t;
  };
  template <> struct NbtValueOf<NbtTag::FloatTag> {
      using type = float;
  };
  template <> struct NbtValueOf<NbtTag::DoubleTag> {
      using type = double;
  };
  template <> struct NbtValueOf<NbtTag::ByteArrayTag> {
      using type = std::string;
  };
  template <> struct NbtValueOf<NbtTag::StringTag> {
      using type = std::vector<int8_t>;
  };
  template <> struct NbtValueOf<NbtTag::ListTag> {
      using type = std::vector<Nbt>;
  };
  template <> struct NbtValueOf<NbtTag::CompoundTag> {
      using type = std::vector<Nbt>;
  };
  template <> struct NbtValueOf<NbtTag::IntArrayTag> {
      using type = std::vector<int64_t>;
  };

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

  Nbt makeNbt(std::string name, const NbtValue& value);
  Nbt makeNbt(const NbtValue& value);

  // void
  // writeNbt(const Nbt& node, std::vector<uint8_t>& data, TagContext context = TagContext::Named);
  // Nbt readNbt(std::span<const uint8_t> data, std::size_t& offset);

  void writeNbtDocument(const Nbt& node, std::vector<uint8_t>& data);
  Nbt readNbtDocument(std::span<const uint8_t> data, std::size_t& offset);
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

      template <typename T> void add(T child) {
        if (_init == false) {
          _tag = NbtTag::CompoundTag;
          _init = true;
          _name = "";
          _value = std::vector<Nbt>();
        }

        auto* values = std::get_if<std::vector<Nbt>>(&_value);

        if (values == nullptr) {
          throw std::runtime_error("invalid container value");
        }

        if constexpr (std::is_same_v<T, Nbt>) {
          values->push_back(std::move(child));
        } else {
          NbtValue value = child;
          values->push_back(makeNbt(value));
        }
      }

      template <typename T> void append(T child) {
        Nbt nbt = [&]() -> Nbt {
          if constexpr (std::is_same_v<T, Nbt>) {
            return std::move(child);
          } else {
            NbtValue value = std::move(child);
            return makeNbt(value);
          }
        }();
        if (_init == false) {
          _tag = NbtTag::ListTag;
          _init = true;
          _name = "";
          _value = std::vector<Nbt>{};
        }

        auto* values = std::get_if<std::vector<Nbt>>(&_value);

        if (values == nullptr) {
          throw std::runtime_error("invalid container value");
        }

        if (_tag == NbtTag::ListTag && !values->empty() && nbt.tag() != values->front().tag()) {
          throw std::runtime_error("all list elements must have the same tag");
        }

        if constexpr (std::is_same_v<T, Nbt>) {
          values->push_back(nbt);
        } else {

          NbtValue value = child;

          values->push_back(makeNbt(value));
        }
      }

      template <NbtTag Tag> const typename NbtValueOf<Tag>::type& get() const {
        assert(_tag == Tag);
        return std::get<typename NbtValueOf<Tag>::type>(_value);
      }

      template <typename T> void setValue(T newValue) {

        if constexpr (std::is_same_v<T, std::vector<Nbt>>) {
          if (!newValue.empty()) {

            const bool allSameTag =
                std::all_of(newValue.begin() + 1, newValue.end(), [&](const Nbt u) {
                  return u.tag() == newValue.front().tag();
                });
            if (allSameTag) {
              _tag = NbtTag::ListTag;
              this->append(newValue);
            } else {
              _tag = NbtTag::CompoundTag;
              this->add(newValue);
            }
          }
        } else if constexpr (std::is_same_v<T, std::vector<Nbt>>) {
          NbtTag tag = tagFromVarible(newValue);
          _tag = tag;
          _value = newValue;
        } else {

          this->append(newValue);
          // throw std::runtime_error("aas");
        }
      }

      /**
       * @brief Compile the NBT tag into binary format and append it to output.
       *
       * @param output The vector to append the serialized bytes to.
       */
      void compile(std::vector<std::uint8_t>& output);
      // void compile(std::vector<std::uint8_t>& output) {
      //   if (_tag != NbtTag::CompoundTag) {
      //     writeNbtDocument(*this, output);
      //   } else {
      //     writeNbt(*this, output);
      //   }
      // }

      /**
       * @brief Compile the NBT tag into binary format.
       *
       * @return std::vector<std::uint8_t> The serialized bytes.
       */
      std::vector<std::uint8_t> compile();
      // {
      //   std::vector<std::uint8_t> output;
      //   compile(output);
      //   return output;
      // }

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
      bool _init = false;
  };

  constexpr std::uint8_t tag(NbtTag value) noexcept;

  // template <NbtTag Tag>
  // constexpr void
  // insertNameAndTag(std::string_view name, std::vector<uint8_t>& data, TagContext context);

} // namespace castle::nbt
