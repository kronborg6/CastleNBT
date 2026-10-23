#include "castle/nbt.hpp"
#include <cstdint>
#include <gtest/gtest.h>
#include <vector>

using castle::nbt::Nbt;
using castle::nbt::NbtTag;

TEST(NbtClass, MakeByte) {
  auto nbt = Nbt::makeByte("health", 20);

  EXPECT_EQ(nbt.tag(), NbtTag::ByteTag);
  EXPECT_EQ(nbt.name(), "health");
  EXPECT_EQ(std::get<std::int8_t>(nbt.value()), 20);
}

TEST(NbtClass, MakeShort) {
  auto nbt = Nbt::makeShort("value", 300);

  EXPECT_EQ(nbt.tag(), NbtTag::ShortTag);
  EXPECT_EQ(nbt.name(), "value");
  EXPECT_EQ(std::get<std::int16_t>(nbt.value()), 300);
}

TEST(NbtClass, MakeInt) {
  auto nbt = Nbt::makeInt("age", 25);

  EXPECT_EQ(nbt.tag(), NbtTag::IntTag);
  EXPECT_EQ(nbt.name(), "age");
  EXPECT_EQ(std::get<std::int32_t>(nbt.value()), 25);
}

TEST(NbtClass, MakeLong) {
  auto nbt = Nbt::makeLong("timestamp", 123456789LL);

  EXPECT_EQ(nbt.tag(), NbtTag::LongTag);
  EXPECT_EQ(nbt.name(), "timestamp");
  EXPECT_EQ(std::get<std::int64_t>(nbt.value()), 123456789LL);
}

TEST(NbtClass, MakeFloat) {
  auto nbt = Nbt::makeFloat("speed", 1.5f);

  EXPECT_EQ(nbt.tag(), NbtTag::FloatTag);
  EXPECT_EQ(nbt.name(), "speed");
  EXPECT_FLOAT_EQ(std::get<float>(nbt.value()), 1.5f);
}

TEST(NbtClass, MakeDouble) {
  auto nbt = Nbt::makeDouble("position", 12.5);

  EXPECT_EQ(nbt.tag(), NbtTag::DoubleTag);
  EXPECT_EQ(nbt.name(), "position");
  EXPECT_DOUBLE_EQ(std::get<double>(nbt.value()), 12.5);
}

TEST(NbtClass, MakeString) {
  auto nbt = Nbt::makeString("name", "Steve");

  EXPECT_EQ(nbt.tag(), NbtTag::StringTag);
  EXPECT_EQ(nbt.name(), "name");
  EXPECT_EQ(std::get<std::string>(nbt.value()), "Steve");
}

TEST(NbtClass, MakeCompound) {
  auto nbt = Nbt::makeCompound("player");

  EXPECT_EQ(nbt.tag(), NbtTag::CompoundTag);
  EXPECT_EQ(nbt.name(), "player");

  const auto& children = std::get<std::vector<Nbt>>(nbt.value());
  EXPECT_TRUE(children.empty());
}

TEST(NbtClass, CompoundAddChild) {
  auto compound = Nbt::makeCompound("player");
  compound.add(Nbt::makeInt("health", 20));

  const auto& children = std::get<std::vector<Nbt>>(compound.value());

  ASSERT_EQ(children.size(), 1);
  EXPECT_EQ(children[0].tag(), NbtTag::IntTag);
  EXPECT_EQ(children[0].name(), "health");
  EXPECT_EQ(std::get<std::int32_t>(children[0].value()), 20);
}

TEST(NbtClass, CompoundAddMultipleChildren) {
  auto compound = Nbt::makeCompound("player");

  compound.add(Nbt::makeInt("health", 20));
  compound.add(Nbt::makeString("name", "Steve"));

  const auto& children = std::get<std::vector<Nbt>>(compound.value());

  ASSERT_EQ(children.size(), 2);
  EXPECT_EQ(children[0].name(), "health");
  EXPECT_EQ(children[1].name(), "name");
}

TEST(NbtClass, MakeList) {
  auto nbt = Nbt::makeList("items");

  EXPECT_EQ(nbt.tag(), NbtTag::ListTag);
  EXPECT_EQ(nbt.name(), "items");

  const auto& elements = std::get<std::vector<Nbt>>(nbt.value());
  EXPECT_TRUE(elements.empty());
}

TEST(NbtClass, ListAppendElements) {
  auto list = Nbt::makeList("numbers");

  list.append(std::int32_t{10});
  list.append(std::int32_t{20});

  const auto& elements = std::get<std::vector<Nbt>>(list.value());

  ASSERT_EQ(elements.size(), 2);
  EXPECT_EQ(std::get<std::int32_t>(elements[0].value()), 10);
  EXPECT_EQ(std::get<std::int32_t>(elements[1].value()), 20);
}

TEST(NbtClass, ListAppendStringKeepsContent) {
  auto list = Nbt::makeList("names");

  list.append(std::string("Steve"));

  const auto& elements = std::get<std::vector<Nbt>>(list.value());

  ASSERT_EQ(elements.size(), 1);
  EXPECT_EQ(elements[0].tag(), NbtTag::StringTag);
  EXPECT_EQ(std::get<std::string>(elements[0].value()), "Steve");
}

TEST(NbtClass, ListAppendIntArrayKeepsContent) {
  auto list = Nbt::makeList("arrays");

  list.append(std::vector<std::int32_t>{1, 2, 3});

  const auto& elements = std::get<std::vector<Nbt>>(list.value());

  ASSERT_EQ(elements.size(), 1);
  EXPECT_EQ(elements[0].tag(), NbtTag::IntArrayTag);
  EXPECT_EQ(std::get<std::vector<std::int32_t>>(elements[0].value()),
            (std::vector<std::int32_t>{1, 2, 3}));
}

TEST(NbtClass, ListRejectsDifferentTypes) {
  auto list = Nbt::makeList("items");
  list.append(Nbt::makeInt("", 10));

  EXPECT_THROW(list.append(Nbt::makeString("", "hello")), std::runtime_error);

  // A failed append must not modify the list.
  const auto& elements = std::get<std::vector<Nbt>>(list.value());
  ASSERT_EQ(elements.size(), 1);
  EXPECT_EQ(elements[0].tag(), NbtTag::IntTag);
}

TEST(NbtClass, Rename) {
  auto nbt = Nbt::makeInt("oldName", 42);

  nbt.rename("newName");

  EXPECT_EQ(nbt.name(), "newName");
  EXPECT_EQ(nbt.tag(), NbtTag::IntTag);
  EXPECT_EQ(std::get<std::int32_t>(nbt.value()), 42);
}

TEST(NbtClass, ClearName) {
  auto nbt = Nbt::makeString("name", "Steve");

  nbt.clearName();

  EXPECT_TRUE(nbt.name().empty());
  EXPECT_EQ(std::get<std::string>(nbt.value()), "Steve");
}

TEST(NbtClass, RenameToEmptyString) {
  auto nbt = Nbt::makeInt("value", 42);

  nbt.rename("");

  EXPECT_TRUE(nbt.name().empty());
}
TEST(NbtClass, MakeByteArray) {
  auto nbt = Nbt::makeByteArray("bytes", {1, -2, 3});

  EXPECT_EQ(nbt.tag(), NbtTag::ByteArrayTag);
  EXPECT_EQ(nbt.name(), "bytes");
  EXPECT_EQ(std::get<std::vector<std::int8_t>>(nbt.value()), (std::vector<std::int8_t>{1, -2, 3}));
}

TEST(NbtClass, MakeIntArray) {
  auto nbt = Nbt::makeIntArray("numbers", {1, 256, -10});

  EXPECT_EQ(nbt.tag(), NbtTag::IntArrayTag);
  EXPECT_EQ(std::get<std::vector<std::int32_t>>(nbt.value()),
            (std::vector<std::int32_t>{1, 256, -10}));
}

TEST(NbtClass, MakeLongArray) {
  auto nbt = Nbt::makeLongArray("numbers", {1, 256});

  EXPECT_EQ(nbt.tag(), NbtTag::LongArrayTag);
  EXPECT_EQ(std::get<std::vector<std::int64_t>>(nbt.value()), (std::vector<std::int64_t>{1, 256}));
}

TEST(NbtClass, EmptyArrays) {
  auto bytes = Nbt::makeByteArray("bytes");
  auto ints = Nbt::makeIntArray("ints");
  auto longs = Nbt::makeLongArray("longs");

  EXPECT_TRUE(std::get<std::vector<std::int8_t>>(bytes.value()).empty());
  EXPECT_TRUE(std::get<std::vector<std::int32_t>>(ints.value()).empty());
  EXPECT_TRUE(std::get<std::vector<std::int64_t>>(longs.value()).empty());
}
