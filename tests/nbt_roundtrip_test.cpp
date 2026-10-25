#include "castle/nbt.hpp"
#include "castle/reader.hpp"
#include "castle/writer.hpp"

#include <cstdint>
#include <gtest/gtest.h>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

using castle::nbt::Nbt;
using castle::nbt::NbtReader;
using castle::nbt::NbtTag;
using castle::nbt::NbtWriter;
using castle::nbt::ReadOptions;
using castle::nbt::WriteOptions;

namespace {

  // Nbt has no operator==, so compare through the writer:
  // if write(read(write(x))) == write(x), nothing was lost or changed on the way.
  void expectRoundTrip(const Nbt& root, WriteOptions writeOptions = {}, ReadOptions readOptions = {}) {
    const std::vector<std::uint8_t> first = NbtWriter::writer(root, writeOptions);
    const Nbt readBack = NbtReader::reader(first, readOptions);
    const std::vector<std::uint8_t> second = NbtWriter::writer(readBack, writeOptions);

    EXPECT_EQ(first, second);
  }

  const Nbt& onlyChild(const Nbt& compound) {
    const auto& children = std::get<std::vector<Nbt>>(compound.value());
    if (children.size() != 1) {
      throw std::runtime_error("expected exactly one child");
    }
    return children.front();
  }

} // namespace

// ---- round trips: write -> read -> write gives the same bytes ----

TEST(NbtRoundTrip, EmptyRootCompound) {
  expectRoundTrip(Nbt::makeCompound(""));
}

TEST(NbtRoundTrip, RootNameIsKept) {
  expectRoundTrip(Nbt::makeCompound("level"));
}

TEST(NbtRoundTrip, AllScalarTypes) {
  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeByte("byte", -5));
  root.add(Nbt::makeShort("short", 1234));
  root.add(Nbt::makeInt("int", -123456));
  root.add(Nbt::makeLong("long", 1234567890123LL));
  root.add(Nbt::makeFloat("float", 3.25f));
  root.add(Nbt::makeDouble("double", -0.125));
  root.add(Nbt::makeString("string", "CastleNBT"));

  expectRoundTrip(root);
}

TEST(NbtRoundTrip, MinAndMaxValues) {
  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeByte("byteMin", std::numeric_limits<std::int8_t>::min()));
  root.add(Nbt::makeByte("byteMax", std::numeric_limits<std::int8_t>::max()));
  root.add(Nbt::makeShort("shortMin", std::numeric_limits<std::int16_t>::min()));
  root.add(Nbt::makeShort("shortMax", std::numeric_limits<std::int16_t>::max()));
  root.add(Nbt::makeInt("intMin", std::numeric_limits<std::int32_t>::min()));
  root.add(Nbt::makeInt("intMax", std::numeric_limits<std::int32_t>::max()));
  root.add(Nbt::makeLong("longMin", std::numeric_limits<std::int64_t>::min()));
  root.add(Nbt::makeLong("longMax", std::numeric_limits<std::int64_t>::max()));

  expectRoundTrip(root);
}

TEST(NbtRoundTrip, FloatSpecialValues) {
  // comparing bytes (not values) also works for NaN, where NaN != NaN
  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeFloat("inf", std::numeric_limits<float>::infinity()));
  root.add(Nbt::makeFloat("negInf", -std::numeric_limits<float>::infinity()));
  root.add(Nbt::makeFloat("negZero", -0.0f));
  root.add(Nbt::makeFloat("nan", std::numeric_limits<float>::quiet_NaN()));
  root.add(Nbt::makeDouble("dInf", std::numeric_limits<double>::infinity()));
  root.add(Nbt::makeDouble("dNegZero", -0.0));
  root.add(Nbt::makeDouble("dNan", std::numeric_limits<double>::quiet_NaN()));
  root.add(Nbt::makeDouble("dTiny", std::numeric_limits<double>::denorm_min()));

  expectRoundTrip(root);
}

TEST(NbtRoundTrip, AllArrayTypes) {
  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeByteArray("bytes", {-128, 0, 127}));
  root.add(Nbt::makeIntArray("ints", {std::numeric_limits<std::int32_t>::min(), 0, 42}));
  root.add(Nbt::makeLongArray("longs", {std::numeric_limits<std::int64_t>::max(), -1}));

  expectRoundTrip(root);
}

TEST(NbtRoundTrip, EmptyArraysAndStrings) {
  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeByteArray("bytes"));
  root.add(Nbt::makeIntArray("ints"));
  root.add(Nbt::makeLongArray("longs"));
  root.add(Nbt::makeString("empty", ""));
  root.add(Nbt::makeString("", "empty name"));

  expectRoundTrip(root);
}

TEST(NbtRoundTrip, Utf8Strings) {
  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeString("danish", "æøå ÆØÅ"));
  root.add(Nbt::makeString("emoji", "🏰⛏️"));
  root.add(Nbt::makeString("名前", "name in Japanese"));

  expectRoundTrip(root);
}

TEST(NbtRoundTrip, NestedCompounds) {
  auto position = Nbt::makeCompound("position");
  position.add(Nbt::makeDouble("x", 10.5));
  position.add(Nbt::makeDouble("y", 64.0));

  auto player = Nbt::makeCompound("player");
  player.add(Nbt::makeString("name", "Steve"));
  player.add(std::move(position));
  player.add(Nbt::makeCompound("emptyChild"));

  auto root = Nbt::makeCompound("");
  root.add(std::move(player));
  root.add(Nbt::makeInt("afterNested", 7)); // a sibling after the nested compound

  expectRoundTrip(root);
}

TEST(NbtRoundTrip, ListsOfEveryKind) {
  auto ints = Nbt::makeList("ints");
  ints.append(std::int32_t{1});
  ints.append(std::int32_t{2});

  auto strings = Nbt::makeList("strings");
  strings.append(std::string("a"));
  strings.append(std::string("b"));

  auto compounds = Nbt::makeList("compounds");
  auto item = Nbt::makeCompound("");
  item.add(Nbt::makeString("id", "minecraft:stone"));
  compounds.append(std::move(item));

  auto inner = Nbt::makeList("");
  inner.append(std::int8_t{5});
  auto listOfLists = Nbt::makeList("listOfLists");
  listOfLists.append(std::move(inner));

  auto root = Nbt::makeCompound("");
  root.add(std::move(ints));
  root.add(std::move(strings));
  root.add(std::move(compounds));
  root.add(std::move(listOfLists));
  root.add(Nbt::makeList("empty"));

  expectRoundTrip(root);
}

// ---- options must match on both sides ----

TEST(NbtRoundTrip, NamelessRoot) {
  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeInt("health", 20));

  expectRoundTrip(root, {.namelessRoot = true}, {.namelessRoot = true});
}

TEST(NbtRoundTrip, NamelessRootWrittenButReadAsNamedFails) {
  // the reader would treat the first child's tag byte as the start of a name length
  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeInt("health", 20));

  const auto bytes = NbtWriter::writer(root, {.namelessRoot = true});
  EXPECT_ANY_THROW(NbtReader::reader(bytes));
}

// ---- wrapNonCompoundRoot ----

TEST(NbtRoundTrip, NamedNonCompoundRootIsWrapped) {
  const auto bytes = NbtWriter::writer(Nbt::makeInt("health", 20));
  const Nbt root = NbtReader::reader(bytes);

  ASSERT_EQ(root.tag(), NbtTag::CompoundTag);
  const Nbt& child = onlyChild(root);
  EXPECT_EQ(child.tag(), NbtTag::IntTag);
  EXPECT_EQ(child.name(), "health");
  EXPECT_EQ(std::get<std::int32_t>(child.value()), 20);
}

TEST(NbtRoundTrip, UnnamedNonCompoundRootIsWrappedAsValue) {
  const auto bytes = NbtWriter::writer(Nbt::makeString("", "hello"));
  const Nbt root = NbtReader::reader(bytes);

  ASSERT_EQ(root.tag(), NbtTag::CompoundTag);
  const Nbt& child = onlyChild(root);
  EXPECT_EQ(child.name(), "value");
  EXPECT_EQ(std::get<std::string>(child.value()), "hello");
}

TEST(NbtRoundTrip, BareNonCompoundRootCanBeReadBack) {
  // DESIGN DECISION: the writer can produce this, but the reader rejects non-compound roots.
  // Either make the reader accept it, or remove/restrict wrapNonCompoundRoot = false
  // and change this test to match.
  const auto bytes = NbtWriter::writer(Nbt::makeString("greeting", "hello"),
                                       {.wrapNonCompoundRoot = false});

  const Nbt root = NbtReader::reader(bytes); // if this throws, gtest reports the test as failed
  EXPECT_EQ(root.tag(), NbtTag::StringTag);
  EXPECT_EQ(root.name(), "greeting");
  EXPECT_EQ(std::get<std::string>(root.value()), "hello");
}

// ---- writer limits: NBT stores string and name lengths as an unsigned 16-bit number ----

TEST(NbtWriterLimits, StringOfMaxLengthIsWritten) {
  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeString("s", std::string(65535, 'a')));

  EXPECT_NO_THROW(NbtWriter::writer(root));
}

TEST(NbtWriterLimits, StringLongerThanMaxThrows) {
  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeString("s", std::string(65536, 'a')));

  EXPECT_ANY_THROW(NbtWriter::writer(root));
}

TEST(NbtWriterLimits, NameLongerThanMaxThrows) {
  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeInt(std::string(65536, 'n'), 1));

  EXPECT_ANY_THROW(NbtWriter::writer(root));
}

TEST(NbtWriterLimits, MaxLengthStringRoundTrips) {
  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeString("s", std::string(65535, 'a')));

  expectRoundTrip(root);
}

TEST(NbtRoundTrip, NamelessBareStringRoot) {
  // network NBT (1.20.2+), e.g. a text component: type byte, then the payload, no name
  const auto bytes = NbtWriter::writer(Nbt::makeString("", "hello"),
                                       {.namelessRoot = true, .wrapNonCompoundRoot = false});

  const std::vector<std::uint8_t> expected = {0x08, 0x00, 0x05, 'h', 'e', 'l', 'l', 'o'};
  EXPECT_EQ(bytes, expected);

  const Nbt root = NbtReader::reader(expected, {.namelessRoot = true});
  EXPECT_EQ(root.tag(), NbtTag::StringTag);
  EXPECT_EQ(std::get<std::string>(root.value()), "hello");
}

TEST(NbtRoundTrip, BareRootInsidePacketStopsAtItsEnd) {
  // a bare root followed by more packet data: the reader must stop after the root
  std::vector<std::uint8_t> bytes = NbtWriter::writer(Nbt::makeInt("i", 7),
                                                      {.wrapNonCompoundRoot = false});
  const std::size_t rootSize = bytes.size();
  bytes.insert(bytes.end(), {0x01, 0x00, 0x01, 'b', 0x05}); // looks like another tag

  std::size_t offset = 0;
  const Nbt root = NbtReader::reader(bytes, offset, {.allowTrailingBytes = true});

  EXPECT_EQ(root.tag(), NbtTag::IntTag);
  EXPECT_EQ(std::get<std::int32_t>(root.value()), 7);
  EXPECT_EQ(offset, rootSize);
}
