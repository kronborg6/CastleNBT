#include "castle/readTag.hpp"

#include <cstddef>
#include <cstdint>
#include <gtest/gtest.h>
#include <span>
#include <vector>

using castle::nbt::Nbt;

TEST(NbtDeserialization, Byte) {
  const std::vector<std::uint8_t> data = {
      0x0A,
      0x00,
      0x00, // Root compound
      0x01,
      0x00,
      0x04, // TAG_Byte, name length = 4
      0x74,
      0x65,
      0x73,
      0x74, // "test"
      0x7F, // 127
      0x00  // TAG_End
  };

  std::size_t offset = 3;

  Nbt tag = castle::nbt::FromTagByte(data, offset);

  EXPECT_EQ(offset, data.size() - 1);

  auto root = Nbt::makeCompound("");
  root.add(std::move(tag));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, data);
}

TEST(NbtDeserialization, Short) {
  const std::vector<std::uint8_t> data = {0x0A,
                                          0x00,
                                          0x00,
                                          0x02,
                                          0x00,
                                          0x04,
                                          0x74,
                                          0x65,
                                          0x73,
                                          0x74,
                                          0x7F,
                                          0xFF, // 32767
                                          0x00};

  std::size_t offset = 3;

  Nbt tag = castle::nbt::FromTagShort(data, offset);

  EXPECT_EQ(offset, data.size() - 1);

  auto root = Nbt::makeCompound("");
  root.add(std::move(tag));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, data);
}

TEST(NbtDeserialization, Int) {
  const std::vector<std::uint8_t> data = {0x0A,
                                          0x00,
                                          0x00,
                                          0x03,
                                          0x00,
                                          0x04,
                                          0x74,
                                          0x65,
                                          0x73,
                                          0x74,
                                          0x7F,
                                          0xFF,
                                          0xFF,
                                          0xFF, // 2147483647
                                          0x00};

  std::size_t offset = 3;

  Nbt tag = castle::nbt::FromTagInt(data, offset);

  EXPECT_EQ(offset, data.size() - 1);

  auto root = Nbt::makeCompound("");
  root.add(std::move(tag));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, data);
}

TEST(NbtDeserialization, Long) {
  // clang-format off
  const std::vector<std::uint8_t> data = {
      0x0A, 0x00, 0x00,
      0x04, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74,
      0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // INT64_MAX
      0x00
  };
  // clang-format on

  std::size_t offset = 3;

  Nbt tag = castle::nbt::FromTagLong(data, offset);

  EXPECT_EQ(offset, data.size() - 1);

  auto root = Nbt::makeCompound("");
  root.add(std::move(tag));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, data);
}

TEST(NbtDeserialization, Float) {
  // clang-format off
  const std::vector<std::uint8_t> data = {
      0x0A, 0x00, 0x00,
      0x05, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74,
      0x3F, 0x00, 0x00, 0x00, // 0.5f
      0x00
  };
  // clang-format on

  std::size_t offset = 3;

  Nbt tag = castle::nbt::FromTagFloat(data, offset);

  EXPECT_EQ(offset, data.size() - 1);

  auto root = Nbt::makeCompound("");
  root.add(std::move(tag));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, data);
}

TEST(NbtDeserialization, Double) {
  // clang-format off
  const std::vector<std::uint8_t> data = {
      0x0A, 0x00, 0x00,
      0x06, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74,
      0x3F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0.5
      0x00
  };
  // clang-format on

  std::size_t offset = 3;

  Nbt tag = castle::nbt::FromTagDouble(data, offset);

  EXPECT_EQ(offset, data.size() - 1);

  auto root = Nbt::makeCompound("");
  root.add(std::move(tag));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, data);
}

TEST(NbtDeserialization, ByteArray) {
  // clang-format off
  const std::vector<std::uint8_t> data = {
      0x0A, 0x00, 0x00,
      0x07, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74,
      0x00, 0x00, 0x00, 0x03, // Array length = 3
      0x01, 0x02, 0x03,
      0x00
  };
  // clang-format on

  std::size_t offset = 3;

  Nbt tag = castle::nbt::FromTagByteArray(data, offset);

  EXPECT_EQ(offset, data.size() - 1);

  auto root = Nbt::makeCompound("");
  root.add(std::move(tag));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, data);
}

TEST(NbtDeserialization, String) {
  // clang-format off
  const std::vector<std::uint8_t> data = {
      0x0A, 0x00, 0x00,
      0x08, 0x00, 0x04,                         // TAG_String, name length = 4
      0x74, 0x65, 0x73, 0x74,                   // "test"
      0x00, 0x05,                               // String length = 5
      0x68, 0x65, 0x6C, 0x6C, 0x6F,             // "hello"
      0x00
  };
  // clang-format on

  std::size_t offset = 3;

  Nbt tag = castle::nbt::FromTagString(data, offset);

  EXPECT_EQ(offset, data.size() - 1);

  auto root = Nbt::makeCompound("");
  root.add(std::move(tag));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, data);
}
TEST(NbtDeserialization, IntArray) {
  // clang-format off
  const std::vector<std::uint8_t> data = {
      0x0A, 0x00, 0x00,                               // Root compound
      0x0B, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74,       // TAG_Int_Array "test"
      0x00, 0x00, 0x00, 0x03,                         // Array length = 3
      0x00, 0x00, 0x00, 0x01,                         // 1
      0x00, 0x00, 0x01, 0x00,                         // 256
      0xFF, 0xFF, 0xFF, 0xFF,                         // -1
      0x00                                            // TAG_End
  };
  // clang-format on

  std::size_t offset = 3;

  Nbt tag = castle::nbt::FromTagIntArray(data, offset);

  EXPECT_EQ(offset, data.size() - 1);

  auto root = Nbt::makeCompound("");
  root.add(std::move(tag));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, data);
}
TEST(NbtDeserialization, LongArray) {
  // clang-format off
  const std::vector<std::uint8_t> data = {
      0x0A, 0x00, 0x00,                               // Root compound
      0x0C, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74,       // TAG_Long_Array "test"
      0x00, 0x00, 0x00, 0x03,                         // Array length = 3
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, // 1
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, // 256
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // -1
      0x00                                            // TAG_End
  };
  // clang-format on

  std::size_t offset = 3;
  std::span<const std::uint8_t> bytes(data);

  Nbt tag = castle::nbt::FromTagLongArray(bytes, offset);

  EXPECT_EQ(offset, data.size() - 1);

  auto root = Nbt::makeCompound("");
  root.add(std::move(tag));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, data);
}
