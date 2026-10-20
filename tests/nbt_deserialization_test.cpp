#include "castle/lowlevel/readTag.hpp"

#include <cstddef>
#include <cstdint>
#include <gtest/gtest.h>
#include <random>
#include <span>
#include <vector>

using castle::nbt::Nbt;
namespace testdata {

  // Hand-written bytes for the AllTagTypes document, independent of the writer.
  inline const std::vector<std::uint8_t>& allTagTypes() {
    // clang-format off
    static const std::vector<std::uint8_t> bytes = {
      0x0A, 0x00, 0x00,                                                                                 // Root compound

      0x01, 0x00, 0x04, 0x62, 0x79, 0x74, 0x65,                                         // TAG_Byte "byte"
      0x7F,                                                                                                    // 127

      0x02, 0x00, 0x05, 0x73, 0x68, 0x6F, 0x72, 0x74,                       // TAG_Short "short"
      0x7F, 0xFF,                                                                                         // 32767

      0x03, 0x00, 0x03, 0x69, 0x6E, 0x74,                                             // TAG_Int "int"
      0x7F, 0xFF, 0xFF, 0xFF,                                                                   // 2147483647

      0x04, 0x00, 0x04, 0x6C, 0x6F, 0x6E, 0x67,                                  // TAG_Long "long"
      0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                       // INT64_MAX

      0x05, 0x00, 0x05, 0x66, 0x6C, 0x6F, 0x61, 0x74,                       // TAG_Float "float"
      0x3F, 0x00, 0x00, 0x00,                                                                   // 0.5f

      0x06, 0x00, 0x06, 0x64, 0x6F, 0x75, 0x62, 0x6C, 0x65,            // TAG_Double "double"
      0x3F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                       // 0.5

      0x07, 0x00, 0x09,                                                                              // TAG_Byte_Array
      0x62, 0x79, 0x74, 0x65, 0x41, 0x72, 0x72, 0x61, 0x79,            // "byteArray"
      0x00, 0x00, 0x00, 0x03,                                                                   // Length = 3
      0x01, 0x02, 0x03,                                                                              // Values

      0x08, 0x00, 0x06,                                                                              // TAG_String
      0x73, 0x74, 0x72, 0x69, 0x6E, 0x67,                                          // "string"
      0x00, 0x05,                                                                                       // Length = 5
      0x68, 0x65, 0x6C, 0x6C, 0x6F,                                                   // "hello"

      0x09, 0x00, 0x04, 0x6C, 0x69, 0x73, 0x74,                           // TAG_List "list"
      0x0A,                                                                                                   // Element type = TAG_Compound
      0x00, 0x00, 0x00, 0x02,                                                               // List length = 2

                                                                                                                    // First compound
      0x08, 0x00, 0x04,                                                                           // TAG_String, name length = 4
      0x6E, 0x61, 0x6D, 0x65,                                                               // "name"
      0x00, 0x05,                                                                                       // String length = 5
      0x41, 0x6C, 0x69, 0x63, 0x65,                                                   // "Alice"

      0x03, 0x00, 0x03,                                                                           // TAG_Int, name length = 3
      0x61, 0x67, 0x65,                                                                           // "age"
      0x00, 0x00, 0x00, 0x19,                                                               // 25
      0x00,                                                                                                   // End first compound

                                                                                                                    // Second compound
      0x08, 0x00, 0x04,                                                                           // TAG_String, name length = 4
      0x6E, 0x61, 0x6D, 0x65,                                                               // "name"
      0x00, 0x03,                                                                                       // String length = 3
      0x42, 0x6F, 0x62,                                                                           // "Bob"

      0x03, 0x00, 0x03,                                                                           // TAG_Int, name length = 3
      0x61, 0x67, 0x65,                                                                           // "age"
      0x00, 0x00, 0x00, 0x1E,                                                               // 30
      0x00,                                                                                                   // End second compound

      0x0A, 0x00, 0x08,                                                                           // TAG_Compound
      0x63, 0x6F, 0x6D, 0x70, 0x6F, 0x75, 0x6E, 0x64,               // "compound"
      0x01, 0x00, 0x01, 0x78, 0x2A,                                                   // TAG_Byte "x" = 42
      0x00,                                                                                                   // End nested compound

      0x0B, 0x00, 0x08,                                                                           // TAG_Int_Array
      0x69, 0x6E, 0x74, 0x41, 0x72, 0x72, 0x61, 0x79,               // "intArray"
      0x00, 0x00, 0x00, 0x02,                                                               // Length = 2
      0x00, 0x00, 0x00, 0x01,                                                               // 1
      0x00, 0x00, 0x01, 0x00,                                                               // 256

      0x0C, 0x00, 0x09,                                                                           // TAG_Long_Array
      0x6C, 0x6F, 0x6E, 0x67, 0x41, 0x72, 0x72, 0x61, 0x79,   // "longArray"
      0x00, 0x00, 0x00, 0x02,                                                               // Length = 2
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,               // 1
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,               // 256

      0x00                                                                                                    // End root compound
    };
    // clang-format on
    return bytes;
  }

} // namespace testdata

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
TEST(NbtDeserialization, ListWith2Int) {
  const std::vector<std::uint8_t> data = {
      0x0a, 0x00, 0x00, 0x09, 0x00, 0x05, 0x6e, 0x61, 0x6d, 0x65, 0x73, 0x08, 0x00,
      0x00, 0x00, 0x04, 0x00, 0x06, 0x4d, 0x69, 0x6b, 0x6b, 0x65, 0x6c, 0x00, 0x04,
      0x54, 0x69, 0x6e, 0x61, 0x00, 0x08, 0x4b, 0x72, 0x69, 0x73, 0x74, 0x69, 0x61,
      0x6e, 0x00, 0x08, 0x6b, 0x72, 0x6f, 0x6e, 0x62, 0x6f, 0x72, 0x67, 0x00};

  std::size_t offset = 3;
  std::span<const std::uint8_t> bytes(data);

  Nbt tag = castle::nbt::FromTagList(bytes, offset);

  EXPECT_EQ(offset, data.size() - 1);

  auto root = Nbt::makeCompound("");
  root.add(std::move(tag));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, data);
}
TEST(NbtDeserialization, HelloWorldFromSpec) {
  const std::vector<std::uint8_t> data = {
      0x0A, 0x00, 0x0B, 'h', 'e',  'l',  'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', 0x08, 0x00, 0x04,
      'n',  'a',  'm',  'e', 0x00, 0x09, 'B', 'a', 'n', 'a', 'n', 'r', 'a', 'm', 'a',  0x00};
  std::size_t offset = 0;
  Nbt tag = castle::nbt::FromTagCompound(data, offset);

  EXPECT_EQ(tag.name(), "hello world");
  EXPECT_EQ(offset, data.size());

  std::vector<std::uint8_t> result;
  tag.compile(result);
  EXPECT_EQ(result, data);
}
TEST(NbtDeserialization, EveryTruncationThrows) {
  const auto& full = testdata::allTagTypes();
  for (std::size_t len = 0; len < full.size(); ++len) {
    std::vector<std::uint8_t> cut(full.begin(), full.begin() + len);
    std::size_t offset = 0;
    EXPECT_THROW(castle::nbt::FromTagCompound(cut, offset), std::runtime_error) << "length " << len;
  }
}

TEST(NbtDeserialization, CorruptedInputNeverCrashes) {
  const auto& full = testdata::allTagTypes();
  std::mt19937 rng(12345); // fixed seed, so it's deterministic
  for (int i = 0; i < 20000; ++i) {
    auto copy = full;
    copy[rng() % copy.size()] = static_cast<std::uint8_t>(rng());
    std::size_t offset = 0;
    try {
      castle::nbt::FromTagCompound(copy, offset);
    } catch (const std::runtime_error&) {
    } // throwing is fine, crashing or other exceptions are not
  }
}
