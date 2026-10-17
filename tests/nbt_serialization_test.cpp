#include "castle/nbt.hpp"
#include <cstdint>
#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>
using castle::nbt::Nbt;
TEST(NbtSerialization, EmptyRootCompound) {

  // clang-format off
  std::vector<std::uint8_t> expted = {
      0x0a,     // Root compound
      0x00,     // Frist child tag
      0x00,     // Name length
      0x00      // End compound
  };
  // clang-format on
  Nbt root = castle::nbt::Nbt::makeCompound("");

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, expted);
}

TEST(NbtSerialization, CompoundWithString) {
  // clang-format off
  const std::vector<std::uint8_t> expected = {
      0x0A, 0x00, 0x00,                             // Root compound
      0x08,                                                 // String tag
      0x00, 0x05,                                       // Name length
      0x68, 0x65, 0x6C, 0x6C, 0x6F,        // "hello"
      0x00, 0x05,                                     // String length
      0x77, 0x6F, 0x72, 0x6C, 0x64,    // "world"
      0x00                                                 // End compound
  };
  // clang-format on

  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeString("hello", "world"));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, expected);
}
TEST(NbtSerialization, Byte) {
  // clang-format off
  const std::vector<std::uint8_t> expected = {
      0x0A, 0x00, 0x00,                 // Root compound
      0x01, 0x00, 0x04,                 // TAG_Byte, name length 4
      0x74, 0x65, 0x73, 0x74,       // "test"
      0x7F,                                    // 127
      0x00                                     // TAG_End
  };

  // clang-format on
  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeByte("test", 127));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, expected);
}

TEST(NbtSerialization, Short) {
  // clang-format off
  const std::vector<std::uint8_t> expected = {
      0x0A, 0x00, 0x00,
      0x02, 0x00, 0x04,
      0x74, 0x65, 0x73, 0x74,
      0x7F, 0xFF,                             // 32767
      0x00
  };
  // clang-format on

  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeShort("test", 32767));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, expected);
}

TEST(NbtSerialization, Int) {
  // clang-format off
  const std::vector<std::uint8_t> expected = {
      0x0A, 0x00, 0x00,
      0x03, 0x00, 0x04,
      0x74, 0x65, 0x73, 0x74,
      0x7F, 0xFF, 0xFF, 0xFF,       // 2147483647
      0x00
  };
  // clang-format on

  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeInt("test", 2147483647));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, expected);
}

TEST(NbtSerialization, Long) {
  // clang-format off
  const std::vector<std::uint8_t> expected = {
      0x0A, 0x00, 0x00,
      0x04, 0x00, 0x04,
      0x74, 0x65, 0x73, 0x74,
      0x7F, 0xFF, 0xFF, 0xFF,
      0xFF, 0xFF, 0xFF, 0xFF,       // INT64_MAX
      0x00
  };
  // clang-format on

  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeLong("test", INT64_MAX));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, expected);
}

TEST(NbtSerialization, Float) {
  // clang-format off
  const std::vector<std::uint8_t> expected = {
      0x0A, 0x00, 0x00,
      0x05, 0x00, 0x04,
      0x74, 0x65, 0x73, 0x74,
      0x3F, 0x00, 0x00, 0x00,       // 0.5f
      0x00
  };
  // clang-format on

  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeFloat("test", 0.5f));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, expected);
}

TEST(NbtSerialization, Double) {
  // clang-format off
  const std::vector<std::uint8_t> expected = {
      0x0A, 0x00, 0x00,
      0x06, 0x00, 0x04,
      0x74, 0x65, 0x73, 0x74,
      0x3F, 0xE0, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00,       // 0.5
      0x00
  };
  // clang-format on

  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeDouble("test", 0.5));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, expected);
}
TEST(NbtSerialization, ByteArray) {
  // clang-format off
  const std::vector<std::uint8_t> expected = {
      0x0A, 0x00, 0x00,
      0x07, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74,
      0x00, 0x00, 0x00, 0x03,                               // Array length = 3
      0x01, 0x02, 0x03,
      0x00
  };
  // clang-format on

  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeByteArray("test", {1, 2, 3}));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, expected);
}

TEST(NbtSerialization, String) {
  // clang-format off
  const std::vector<std::uint8_t> expected = {
      0x0A, 0x00, 0x00,
      0x08, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74,
      0x00, 0x05,                                                     // String length = 5
      0x68, 0x65, 0x6C, 0x6C, 0x6F,                    // "hello"
      0x00
  };
  // clang-format on

  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeString("test", "hello"));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, expected);
}

TEST(NbtSerialization, List) {
  // clang-format off
  const std::vector<std::uint8_t> expected = {
      0x0A, 0x00, 0x00,
      0x09, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74,
      0x03,                                                                // List element type TAG_Int
      0x00, 0x00, 0x00, 0x02,                               // List length = 2
      0x00, 0x00, 0x00, 0x01,                               // First int
      0x00, 0x00, 0x00, 0x02,                               // Second int
      0x00
  };
  // clang-format on

  auto root = Nbt::makeCompound("");
  auto list = Nbt::makeList("test");

  list.append(std::int32_t{1});
  list.append(std::int32_t{2});

  root.add(std::move(list));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, expected);
}

TEST(NbtSerialization, ListWithCompounds) {
  // clang-format off
  const std::vector<std::uint8_t> expected = {
      0x0A, 0x00, 0x00,                                             // Root compound
      0x09, 0x00, 0x04, 0x6C, 0x69, 0x73, 0x74,     // List "list"
      0x0A,                                                                // Element type: compound
      0x00, 0x00, 0x00, 0x02,                               // Two elements

      0x00,                                                                // First empty compound
      0x00,                                                                // Second empty compound
      0x00                                                                 // End root compound
  };
  // clang-format on

  auto root = Nbt::makeCompound("");
  auto list = Nbt::makeList("list");

  list.append(Nbt::makeCompound("this does not matter becus is gona be set to emty"));
  list.append(Nbt::makeCompound("this does not matter becus is gona be set to emty"));

  root.add(std::move(list));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, expected);
}
TEST(NbtSerialization, EmptyList) {
  // clang-format off
  const std::vector<std::uint8_t> expected = {
      0x0A, 0x00, 0x00,                                             // Root compound
      0x09, 0x00, 0x04, 0x6C, 0x69, 0x73, 0x74,     // List "list"
      0x00,                                                                // TAG_End element type
      0x00, 0x00, 0x00, 0x00,                               // Length = 0
      0x00                                                                 // End root compound
  };
  // clang-format on

  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeList("list"));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, expected);
}

TEST(NbtSerialization, ListRejectsDifferentTypes) {
  auto list = Nbt::makeList("list");

  list.append(Nbt::makeCompound(""));

  EXPECT_THROW(list.append(Nbt::makeString("this does not matter")), std::runtime_error);
}

TEST(NbtSerialization, Compound) {
  // clang-format off
  const std::vector<std::uint8_t> expected = {
      0x0A, 0x00, 0x00,
      0x0A, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74,
      0x01, 0x00, 0x01, 0x78, 0x2A,                    // Byte "x" = 42
      0x00,                                                                // End nested compound
      0x00                                                                 // End root compound
  };
  // clang-format on

  auto root = Nbt::makeCompound("");
  auto compound = Nbt::makeCompound("test");

  compound.add(Nbt::makeByte("x", 42));
  root.add(std::move(compound));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, expected);
}

TEST(NbtSerialization, IntArray) {
  // clang-format off
  const std::vector<std::uint8_t> expected = {
      0x0A, 0x00, 0x00,
      0x0B, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74,
      0x00, 0x00, 0x00, 0x02,                               // Array length = 2
      0x00, 0x00, 0x00, 0x01,
      0x00, 0x00, 0x01, 0x00,                               // 256
      0x00
  };
  // clang-format on

  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeIntArray("test", {1, 256}));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, expected);
}

TEST(NbtSerialization, LongArray) {
  // clang-format off
  const std::vector<std::uint8_t> expected = {
      0x0A, 0x00, 0x00,
      0x0C, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74,
      0x00, 0x00, 0x00, 0x02,                                               // Array length = 2
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
      0x00
  };
  // clang-format on

  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeLongArray("test", {1, 256}));

  std::vector<std::uint8_t> result;
  root.compile(result);

  EXPECT_EQ(result, expected);
}
TEST(NbtSerialization, AllTagTypes) {
  // clang-format off
  const std::vector<std::uint8_t> expected = {
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

  auto root = Nbt::makeCompound("");

  // Numeric tags
  root.add(Nbt::makeByte("byte", 127));
  root.add(Nbt::makeShort("short", 32767));
  root.add(Nbt::makeInt("int", 2147483647));
  root.add(Nbt::makeLong("long", INT64_MAX));
  root.add(Nbt::makeFloat("float", 0.5f));
  root.add(Nbt::makeDouble("double", 0.5));

  // Byte array
  root.add(Nbt::makeByteArray("byteArray", {1, 2, 3}));

  // String
  root.add(Nbt::makeString("string", "hello"));

  // List of compounds
  auto list = Nbt::makeList("list");

  auto alice = Nbt::makeCompound("");
  alice.add(Nbt::makeString("name", "Alice"));
  alice.add(Nbt::makeInt("age", 25));

  auto bob = Nbt::makeCompound("");
  bob.add(Nbt::makeString("name", "Bob"));
  bob.add(Nbt::makeInt("age", 30));

  list.append(std::move(alice));
  list.append(std::move(bob));

  root.add(std::move(list));

  // Nested compound
  auto compound = Nbt::makeCompound("compound");
  compound.add(Nbt::makeByte("x", 42));
  root.add(std::move(compound));

  // Arrays
  root.add(Nbt::makeIntArray("intArray", {1, 256}));
  root.add(Nbt::makeLongArray("longArray", {1, 256}));

  // Serialize
  std::vector<std::uint8_t> result = root.compile();

  ASSERT_EQ(result.size(), expected.size()) << "Serialized NBT size does not match";

  for (std::size_t i = 0; i < std::min(result.size(), expected.size()); ++i) {
    EXPECT_EQ(result[i], expected[i])
        << "Byte mismatch at offset " << i << " (0x" << std::hex << i << ")";
  }
}
TEST(NbtSerialization, AppendsToExistingBuffer) {
  auto root = Nbt::makeCompound("");

  std::vector<std::uint8_t> result = {0xAA};
  root.compile(result);

  ASSERT_FALSE(result.empty());
  EXPECT_EQ(result.front(), 0xAA);
}
