#include "castle/nbt.hpp"

#include <cstdint>
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

using castle::nbt::Nbt;
using castle::nbt::NbtTag;
using castle::nbt::NbtValueOf;

namespace {

  // true when NbtValueOf<Tag> exists AND maps to Expected.
  // The requires-check means a missing specialization fails this test
  // instead of breaking the whole build.
  template <NbtTag Tag, typename Expected> constexpr bool valueTypeIs() {
    if constexpr (requires { typename NbtValueOf<Tag>::type; }) {
      return std::is_same_v<typename NbtValueOf<Tag>::type, Expected>;
    } else {
      return false;
    }
  }

  // get<Tag>() only compiles when NbtValueOf<Tag> is right, so check that first.
  // if constexpr only skips the bad branch inside a template, which is why this is a template.
  template <NbtTag Tag, typename Expected> void expectGet(const Nbt& nbt, const Expected& expected) {
    if constexpr (valueTypeIs<Tag, Expected>()) {
      EXPECT_EQ(nbt.template get<Tag>(), expected);
    } else {
      ADD_FAILURE() << "NbtValueOf maps this tag to the wrong type, so get<>() can't be used";
    }
  }

} // namespace

// ---- NbtValueOf must agree with the type table in the README ----

TEST(NbtValueOf, Byte) {
  EXPECT_TRUE((valueTypeIs<NbtTag::ByteTag, std::int8_t>()));
}
TEST(NbtValueOf, Short) {
  EXPECT_TRUE((valueTypeIs<NbtTag::ShortTag, std::int16_t>()));
}
TEST(NbtValueOf, Int) {
  EXPECT_TRUE((valueTypeIs<NbtTag::IntTag, std::int32_t>()));
}
TEST(NbtValueOf, Long) {
  EXPECT_TRUE((valueTypeIs<NbtTag::LongTag, std::int64_t>()));
}
TEST(NbtValueOf, Float) {
  EXPECT_TRUE((valueTypeIs<NbtTag::FloatTag, float>()));
}
TEST(NbtValueOf, Double) {
  EXPECT_TRUE((valueTypeIs<NbtTag::DoubleTag, double>()));
}
TEST(NbtValueOf, ByteArray) {
  EXPECT_TRUE((valueTypeIs<NbtTag::ByteArrayTag, std::vector<std::int8_t>>()));
}
TEST(NbtValueOf, String) {
  EXPECT_TRUE((valueTypeIs<NbtTag::StringTag, std::string>()));
}
TEST(NbtValueOf, List) {
  EXPECT_TRUE((valueTypeIs<NbtTag::ListTag, std::vector<Nbt>>()));
}
TEST(NbtValueOf, Compound) {
  EXPECT_TRUE((valueTypeIs<NbtTag::CompoundTag, std::vector<Nbt>>()));
}
TEST(NbtValueOf, IntArray) {
  EXPECT_TRUE((valueTypeIs<NbtTag::IntArrayTag, std::vector<std::int32_t>>()));
}
TEST(NbtValueOf, LongArray) {
  EXPECT_TRUE((valueTypeIs<NbtTag::LongArrayTag, std::vector<std::int64_t>>()));
}

// ---- get<Tag>() returns the stored value ----

TEST(NbtGet, Int) {
  EXPECT_EQ(Nbt::makeInt("i", 42).get<NbtTag::IntTag>(), 42);
}

TEST(NbtGet, Double) {
  EXPECT_EQ(Nbt::makeDouble("d", 0.5).get<NbtTag::DoubleTag>(), 0.5);
}

TEST(NbtGet, String) {
  expectGet<NbtTag::StringTag>(Nbt::makeString("s", "Steve"), std::string("Steve"));
}

TEST(NbtGet, ByteArray) {
  expectGet<NbtTag::ByteArrayTag>(Nbt::makeByteArray("b", {1, 2, 3}),
                                  std::vector<std::int8_t>{1, 2, 3});
}

TEST(NbtGet, IntArray) {
  expectGet<NbtTag::IntArrayTag>(Nbt::makeIntArray("i", {1, 2, 3}),
                                 std::vector<std::int32_t>{1, 2, 3});
}

TEST(NbtGet, LongArray) {
  expectGet<NbtTag::LongArrayTag>(Nbt::makeLongArray("l", {1, 2, 3}),
                                  std::vector<std::int64_t>{1, 2, 3});
}

TEST(NbtGet, CompoundChildren) {
  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeInt("a", 1));
  root.add(Nbt::makeInt("b", 2));

  const auto& children = root.get<NbtTag::CompoundTag>();
  ASSERT_EQ(children.size(), 2u);
  EXPECT_EQ(children[1].name(), "b");
}

// ---- containers ----

TEST(NbtContainer, AddToNonContainerThrows) {
  auto number = Nbt::makeInt("x", 1);
  EXPECT_THROW(number.add(Nbt::makeInt("y", 2)), std::runtime_error);
}

TEST(NbtContainer, AppendToNonContainerThrows) {
  auto text = Nbt::makeString("x", "hello");
  EXPECT_THROW(text.append(std::int32_t{2}), std::runtime_error);
}

TEST(NbtContainer, CompoundAllowsMixedTypes) {
  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeInt("a", 1));
  EXPECT_NO_THROW(root.add(Nbt::makeString("b", "two")));
}

TEST(NbtContainer, AddStringValueKeepsContent) {
  // same pattern as the append() use-after-move bug: check add() keeps the value too
  auto root = Nbt::makeCompound("");
  root.add(std::string("Steve"));

  const auto& children = root.get<NbtTag::CompoundTag>();
  ASSERT_EQ(children.size(), 1u);
  EXPECT_EQ(std::get<std::string>(children[0].value()), "Steve");
}

TEST(NbtContainer, CopyIsIndependent) {
  // copies must be deep: changing the copy must not change the original
  auto original = Nbt::makeCompound("");
  original.add(Nbt::makeInt("a", 1));

  Nbt copy = original;
  copy.add(Nbt::makeInt("b", 2));
  copy.rename("copy");

  EXPECT_EQ(original.get<NbtTag::CompoundTag>().size(), 1u);
  EXPECT_EQ(original.name(), "");
}

TEST(NbtContainer, RenamedLeavesOriginalAlone) {
  auto original = Nbt::makeInt("old", 5);
  Nbt copy = original.renamed("new");

  EXPECT_EQ(original.name(), "old");
  EXPECT_EQ(copy.name(), "new");
  EXPECT_EQ(copy.get<NbtTag::IntTag>(), 5);
}

// ---- setValue ----

TEST(NbtSetValue, ReplacesScalarValue) {
  auto nbt = Nbt::makeInt("x", 1);
  nbt.setValue(std::int32_t{2});

  EXPECT_EQ(nbt.tag(), NbtTag::IntTag);
  EXPECT_EQ(std::get<std::int32_t>(nbt.value()), 2);
}
