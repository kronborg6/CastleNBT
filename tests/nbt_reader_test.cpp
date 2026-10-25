#include "castle/nbt.hpp"
#include "castle/reader.hpp"

#include <cstddef>
#include <cstdint>
#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

using castle::nbt::Nbt;
using castle::nbt::NbtReader;
using castle::nbt::NbtTag;
using castle::nbt::ReadOptions;

namespace {

  // hello_world.nbt from the NBT spec:
  // TAG_Compound("hello world") { TAG_String("name"): "Bananrama" }
  const std::vector<std::uint8_t>& helloWorld() {
    // clang-format off
    static const std::vector<std::uint8_t> bytes = {
      0x0A, 0x00, 0x0B,                                                                                             // TAG_Compound, name length 11
      'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd',                    // "hello world"
      0x08, 0x00, 0x04, 'n', 'a', 'm', 'e',                                                  // TAG_String "name"
      0x00, 0x09, 'B', 'a', 'n', 'a', 'n', 'r', 'a', 'm', 'a',           // "Bananrama"
      0x00,                                                                                                                // TAG_End
    };
    // clang-format on
    return bytes;
  }

  void expectHelloWorld(const Nbt& root) {
    ASSERT_EQ(root.tag(), NbtTag::CompoundTag);
    EXPECT_EQ(root.name(), "hello world");

    const auto* children = std::get_if<std::vector<Nbt>>(&root.value());
    ASSERT_NE(children, nullptr);
    ASSERT_EQ(children->size(), 1u);

    const Nbt& child = children->front();
    EXPECT_EQ(child.tag(), NbtTag::StringTag);
    EXPECT_EQ(child.name(), "name");
    EXPECT_EQ(std::get<std::string>(child.value()), "Bananrama");
  }

  std::istringstream toStream(const std::vector<std::uint8_t>& bytes) {
    return std::istringstream(std::string(bytes.begin(), bytes.end()), std::ios::binary);
  }

} // namespace

// ---- span overload without offset ----

TEST(NbtReader, SpanWithoutOffsetReadsFromStart) {
  expectHelloWorld(NbtReader::reader(helloWorld()));
}

TEST(NbtReader, SpanWithoutOffsetGivesSameResultAsOffsetZero) {
  std::size_t offset = 0;
  Nbt withOffset = NbtReader::reader(helloWorld(), offset);
  Nbt withoutOffset = NbtReader::reader(helloWorld());

  expectHelloWorld(withOffset);
  expectHelloWorld(withoutOffset);
}

TEST(NbtReader, SpanWithoutOffsetEmptyInputThrows) {
  std::vector<std::uint8_t> empty;
  EXPECT_ANY_THROW(NbtReader::reader(empty));
}

// ---- span overload with offset ----

TEST(NbtReader, OffsetAdvancesToEndOfDocument) {
  std::size_t offset = 0;
  NbtReader::reader(helloWorld(), offset);
  EXPECT_EQ(offset, helloWorld().size());
}

TEST(NbtReader, OffsetStartsReadingAtGivenPosition) {
  std::vector<std::uint8_t> bytes = {0xDE, 0xAD, 0xBE}; // junk before the document
  bytes.insert(bytes.end(), helloWorld().begin(), helloWorld().end());

  std::size_t offset = 3;
  expectHelloWorld(NbtReader::reader(bytes, offset));
  EXPECT_EQ(offset, bytes.size());
}

TEST(NbtReader, OffsetCanReadTwoDocumentsBackToBack) {
  std::vector<std::uint8_t> bytes = helloWorld();
  bytes.insert(bytes.end(), helloWorld().begin(), helloWorld().end());

  ReadOptions options;
  options.allowTrailingBytes = true; // the first read leaves the second document behind

  std::size_t offset = 0;
  expectHelloWorld(NbtReader::reader(bytes, offset, options));
  EXPECT_EQ(offset, helloWorld().size());

  expectHelloWorld(NbtReader::reader(bytes, offset, options));
  EXPECT_EQ(offset, bytes.size());
}

TEST(NbtReader, OffsetPastEndThrows) {
  std::size_t offset = helloWorld().size() + 10;
  EXPECT_ANY_THROW(NbtReader::reader(helloWorld(), offset));
}

// ---- istream overload ----

TEST(NbtReader, StreamReadsDocument) {
  auto stream = toStream(helloWorld());
  expectHelloWorld(NbtReader::reader(stream));
}

TEST(NbtReader, StreamWithZeroBytesInsideIsNotCutShort) {
  // helloWorld() contains 0x00 bytes; a text-style read would stop at the first one
  auto stream = toStream(helloWorld());
  Nbt root = NbtReader::reader(stream);
  EXPECT_EQ(std::get<std::vector<Nbt>>(root.value()).size(), 1u);
}

TEST(NbtReader, StreamInBadStateThrows) {
  auto stream = toStream(helloWorld());
  stream.setstate(std::ios::badbit);
  EXPECT_THROW(NbtReader::reader(stream), std::runtime_error);
}

TEST(NbtReader, EmptyStreamThrows) {
  std::istringstream stream("", std::ios::binary);
  EXPECT_ANY_THROW(NbtReader::reader(stream));
}

TEST(NbtReader, TrailingBytes) {
  std::vector<std::uint8_t> bytes; // junk before the document
  bytes.insert(bytes.begin(), helloWorld().begin(), helloWorld().end());
  bytes.insert(bytes.end(), 0x0F);

  std::size_t offset = 0;

  EXPECT_THROW(NbtReader::reader(bytes, offset), std::runtime_error);
}

TEST(NbtReader, TrailingBytesAllowed) {
  std::vector<std::uint8_t> bytes; // junk before the document
  bytes.insert(bytes.begin(), helloWorld().begin(), helloWorld().end());
  bytes.insert(bytes.end(), 0x0F);

  std::size_t offset = 0;

  EXPECT_NO_THROW(NbtReader::reader(bytes, offset, {.allowTrailingBytes = true}));
}

// ---- maxDepth ----

namespace {

  // Root compound holding one list named "l", nested `levels` lists deep:
  // { l: [[[[ ... [] ... ]]]] }
  std::vector<std::uint8_t> nestedLists(std::size_t levels) {
    std::vector<std::uint8_t> bytes = {0x0A, 0x00, 0x00, // root compound, empty name
                                       0x09, 0x00, 0x01, 'l'}; // TAG_List "l"

    for (std::size_t i = 0; i < levels; i++) {
      // this list holds 1 element, and that element is another list
      bytes.insert(bytes.end(), {0x09, 0x00, 0x00, 0x00, 0x01});
    }

    bytes.insert(bytes.end(), {0x00, 0x00, 0x00, 0x00, 0x00}); // innermost list: type End, length 0
    bytes.push_back(0x00);                                      // TAG_End for the root
    return bytes;
  }

} // namespace

TEST(NbtReaderDepth, ShallowNestedListsAreRead) {
  // proves nestedLists() builds valid NBT, so the test below fails for the right reason
  EXPECT_NO_THROW(NbtReader::reader(nestedLists(10)));
}

TEST(NbtReaderDepth, DeeplyNestedListsThrow) {
  // far past the default maxDepth of 512, but shallow enough not to overflow the stack
  EXPECT_THROW(NbtReader::reader(nestedLists(2000)), std::runtime_error);
}

TEST(NbtReaderDepth, SmallMaxDepthIsRespected) {
  // nesting that is fine with the default limit must throw with a smaller one
  ASSERT_NO_THROW(NbtReader::reader(nestedLists(10)));
  EXPECT_THROW(NbtReader::reader(nestedLists(10), {.maxDepth = 5}), std::runtime_error);
}

TEST(NbtReaderDepth, LargerMaxDepthAllowsDeeperNesting) {
  EXPECT_NO_THROW(NbtReader::reader(nestedLists(2000), {.maxDepth = 3000}));
}

TEST(NbtReaderDepth, ListOfEndTagWithLengthThrows) {
  // element type End means "empty list"; a non-zero length is corrupt data
  std::vector<std::uint8_t> bytes = {0x0A, 0x00, 0x00,            // root compound
                                     0x09, 0x00, 0x01, 'l',       // TAG_List "l"
                                     0x00, 0x00, 0x00, 0x00, 0x03, // type End, length 3
                                     0x00};                       // root TAG_End
  EXPECT_THROW(NbtReader::reader(bytes), std::runtime_error);
}

TEST(NbtReader, StreamReadsFromCurrentPosition) {
  // a stream that was partly consumed should be read from where it is, not from the start
  std::vector<std::uint8_t> bytes = {0xDE, 0xAD};
  bytes.insert(bytes.end(), helloWorld().begin(), helloWorld().end());
  auto stream = toStream(bytes);
  stream.ignore(2);

  expectHelloWorld(NbtReader::reader(stream));
}
