#include "castle/lowlevel/readTag.hpp"
#include "castle/writer.hpp"

#include <cstddef>
#include <cstdint>
#include <gtest/gtest.h>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

using castle::nbt::Nbt;
using castle::nbt::NbtTag;
using castle::nbt::TagContext;

namespace {

  // 0A 00 00 <body> 00  -> a nameless-in-file-format ("" name) root compound around `body`
  std::vector<std::uint8_t> makeDoc(const std::vector<std::uint8_t>& body) {
    std::vector<std::uint8_t> doc = {0x0A, 0x00, 0x00};
    doc.insert(doc.end(), body.begin(), body.end());
    doc.push_back(0x00);
    return doc;
  }

  // Reads one tag starting at offset 3 (right after the root header), checks that the whole
  // body was consumed (offset ends on the final TAG_End), then writes it back and compares.
  //
  // `read` can be a 3-parameter reader (byte, int, string, arrays, ...) or the 5-parameter
  // FromTagList / FromTagCompound that also take depth and maxDepth.
  template <typename Reader>
  void expectRoundTrip(const std::vector<std::uint8_t>& data, Reader read) {
    std::size_t offset = 3;

    Nbt tag = [&]() -> Nbt {
      if constexpr (std::is_invocable_v<Reader,
                                        std::span<const std::uint8_t>,
                                        std::size_t&,
                                        TagContext>) {
        return read(data, offset, TagContext::Named);
      } else {
        return read(data, offset, TagContext::Named, 0, 512);
      }
    }();

    EXPECT_EQ(offset, data.size() - 1);

    auto root = Nbt::makeCompound("");
    root.add(std::move(tag));

    std::vector<std::uint8_t> result = castle::nbt::NbtWriter::writer(root);

    EXPECT_EQ(result, data);
  }

} // namespace

// ---------------------------------------------------------------------------
// readTagHeader
// ---------------------------------------------------------------------------

TEST(NbtHeader, NamedReadsNameAndAdvances) {
  const std::vector<std::uint8_t> data = {0x01, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74};
  std::size_t offset = 0;

  std::string name = castle::nbt::readTagHeader(data, offset, NbtTag::ByteTag);

  EXPECT_EQ(name, "test");
  EXPECT_EQ(offset, 7u);
}

TEST(NbtHeader, NamedEmptyName) {
  const std::vector<std::uint8_t> data = {0x01, 0x00, 0x00};
  std::size_t offset = 0;

  std::string name = castle::nbt::readTagHeader(data, offset, NbtTag::ByteTag);

  EXPECT_EQ(name, "");
  EXPECT_EQ(offset, 3u);
}

TEST(NbtHeader, NamelessRootConsumesOnlyTagByte) {
  const std::vector<std::uint8_t> data = {0x0A, 0x01};
  std::size_t offset = 0;

  std::string name =
      castle::nbt::readTagHeader(data, offset, NbtTag::CompoundTag, TagContext::NamelessRoot);

  EXPECT_EQ(name, "");
  EXPECT_EQ(offset, 1u);
}

TEST(NbtHeader, NamelessRootWrongTagThrows) {
  const std::vector<std::uint8_t> data = {0x08};
  std::size_t offset = 0;

  EXPECT_THROW(
      castle::nbt::readTagHeader(data, offset, NbtTag::CompoundTag, TagContext::NamelessRoot),
      std::runtime_error);
}

TEST(NbtHeader, ListElementConsumesNothing) {
  const std::vector<std::uint8_t> data = {0x7F};
  std::size_t offset = 0;

  std::string name =
      castle::nbt::readTagHeader(data, offset, NbtTag::ByteTag, TagContext::ListElement);

  EXPECT_EQ(name, "");
  EXPECT_EQ(offset, 0u);
}

TEST(NbtHeader, EndTagHasNoName) {
  const std::vector<std::uint8_t> data = {0x00};
  std::size_t offset = 0;

  std::string name = castle::nbt::readTagHeader(data, offset, NbtTag::EndTag);

  EXPECT_EQ(name, "");
  EXPECT_EQ(offset, 1u);
}

TEST(NbtHeader, WrongTagTypeThrows) {
  const std::vector<std::uint8_t> data = {0x02, 0x00, 0x00};
  std::size_t offset = 0;

  EXPECT_THROW(castle::nbt::readTagHeader(data, offset, NbtTag::ByteTag), std::runtime_error);
}

TEST(NbtHeader, EmptyDataThrows) {
  const std::vector<std::uint8_t> data = {};
  std::size_t offset = 0;

  EXPECT_THROW(castle::nbt::readTagHeader(data, offset, NbtTag::ByteTag), std::runtime_error);
}

TEST(NbtHeader, OffsetAtEndThrows) {
  const std::vector<std::uint8_t> data = {0x01, 0x00, 0x00};
  std::size_t offset = data.size();

  EXPECT_THROW(castle::nbt::readTagHeader(data, offset, NbtTag::ByteTag), std::runtime_error);
}

TEST(NbtHeader, TruncatedNameLengthThrows) {
  // tag byte + only 1 of the 2 length bytes. Fails if readBigEndian does not bounds-check.
  const std::vector<std::uint8_t> data = {0x01, 0x00};
  std::size_t offset = 0;

  EXPECT_THROW(castle::nbt::readTagHeader(data, offset, NbtTag::ByteTag), std::runtime_error);
}

TEST(NbtHeader, NameLongerThanDataThrows) {
  // says 5 bytes of name, only 2 present
  const std::vector<std::uint8_t> data = {0x01, 0x00, 0x05, 0x61, 0x62};
  std::size_t offset = 0;

  EXPECT_THROW(castle::nbt::readTagHeader(data, offset, NbtTag::ByteTag), std::runtime_error);
}

// ---------------------------------------------------------------------------
// Primitive edge values (round trip)
// ---------------------------------------------------------------------------

TEST(NbtDeserialization, ByteNegative) {
  expectRoundTrip(makeDoc({0x01, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74, 0xFF}), // -1
                  castle::nbt::FromTagByte);
}

TEST(NbtDeserialization, ShortMin) {
  expectRoundTrip(makeDoc({0x02, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74, 0x80, 0x00}), // -32768
                  castle::nbt::FromTagShort);
}

TEST(NbtDeserialization, IntMin) {
  expectRoundTrip(makeDoc({0x03, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74, 0x80, 0x00, 0x00, 0x00}),
                  castle::nbt::FromTagInt);
}

TEST(NbtDeserialization, LongMin) {
  expectRoundTrip(makeDoc({0x04,
                           0x00,
                           0x04,
                           0x74,
                           0x65,
                           0x73,
                           0x74,
                           0x80,
                           0x00,
                           0x00,
                           0x00,
                           0x00,
                           0x00,
                           0x00,
                           0x00}),
                  castle::nbt::FromTagLong);
}

TEST(NbtDeserialization, FloatNegative) {
  expectRoundTrip(
      makeDoc({0x05, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74, 0xBF, 0xC0, 0x00, 0x00}), // -1.5f
      castle::nbt::FromTagFloat);
}

TEST(NbtDeserialization, DoubleNegative) {
  expectRoundTrip(makeDoc({0x06,
                           0x00,
                           0x04,
                           0x74,
                           0x65,
                           0x73,
                           0x74,
                           0xC0,
                           0x00,
                           0x00,
                           0x00,
                           0x00,
                           0x00,
                           0x00,
                           0x00}), // -2.0
                  castle::nbt::FromTagDouble);
}

// ---------------------------------------------------------------------------
// Strings and arrays, including empty ones
// ---------------------------------------------------------------------------

TEST(NbtDeserialization, StringEmpty) {
  expectRoundTrip(makeDoc({0x08, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74, 0x00, 0x00}),
                  castle::nbt::FromTagString);
}

TEST(NbtDeserialization, StringUtf8) {
  // "æøå" = C3 A6 C3 B8 C3 A5, length is in BYTES (6), not characters (3)
  expectRoundTrip(makeDoc({0x08,
                           0x00,
                           0x04,
                           0x74,
                           0x65,
                           0x73,
                           0x74,
                           0x00,
                           0x06,
                           0xC3,
                           0xA6,
                           0xC3,
                           0xB8,
                           0xC3,
                           0xA5}),
                  castle::nbt::FromTagString);
}

TEST(NbtDeserialization, ByteArrayEmpty) {
  expectRoundTrip(makeDoc({0x07, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74, 0x00, 0x00, 0x00, 0x00}),
                  castle::nbt::FromTagByteArray);
}

TEST(NbtDeserialization, IntArrayEmpty) {
  expectRoundTrip(makeDoc({0x0B, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74, 0x00, 0x00, 0x00, 0x00}),
                  castle::nbt::FromTagIntArray);
}

TEST(NbtDeserialization, LongArrayEmpty) {
  expectRoundTrip(makeDoc({0x0C, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74, 0x00, 0x00, 0x00, 0x00}),
                  castle::nbt::FromTagLongArray);
}

// ---------------------------------------------------------------------------
// Bad input on the primitive readers: must throw, never read out of bounds
// ---------------------------------------------------------------------------

TEST(NbtDeserialization, WrongTagTypeThrows) {
  // a TAG_Byte read by the Int reader
  const std::vector<std::uint8_t> data = {0x01, 0x00, 0x01, 0x61, 0x7F};
  std::size_t offset = 0;

  EXPECT_THROW(castle::nbt::FromTagInt(data, offset), std::runtime_error);
}

TEST(NbtDeserialization, IntTruncatedPayloadThrows) {
  const std::vector<std::uint8_t> data = {0x03, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74, 0x7F, 0xFF};
  std::size_t offset = 0;

  EXPECT_THROW(castle::nbt::FromTagInt(data, offset), std::runtime_error);
}

TEST(NbtDeserialization, StringLengthPastEndThrows) {
  const std::vector<std::uint8_t> data = {0x08, 0x00, 0x01, 0x73, 0x00, 0x10, 0x68, 0x69};
  std::size_t offset = 0;

  EXPECT_THROW(castle::nbt::FromTagString(data, offset), std::runtime_error);
}

TEST(NbtDeserialization, ByteArrayHugeLengthThrows) {
  // length 0x7FFFFFFF with 1 byte of data. Must throw, not try to allocate 2 GiB.
  const std::vector<std::uint8_t> data = {0x07, 0x00, 0x01, 0x62, 0x7F, 0xFF, 0xFF, 0xFF, 0x01};
  std::size_t offset = 0;

  EXPECT_THROW(castle::nbt::FromTagByteArray(data, offset), std::runtime_error);
}

TEST(NbtDeserialization, IntArrayHugeLengthThrows) {
  const std::vector<std::uint8_t> data = {
      0x0B, 0x00, 0x01, 0x69, 0x7F, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x01};
  std::size_t offset = 0;

  EXPECT_THROW(castle::nbt::FromTagIntArray(data, offset), std::runtime_error);
}

TEST(NbtDeserialization, LongArrayHugeLengthThrows) {
  const std::vector<std::uint8_t> data = {0x0C,
                                          0x00,
                                          0x01,
                                          0x6C,
                                          0x7F,
                                          0xFF,
                                          0xFF,
                                          0xFF,
                                          0x00,
                                          0x00,
                                          0x00,
                                          0x00,
                                          0x00,
                                          0x00,
                                          0x00,
                                          0x01};
  std::size_t offset = 0;

  EXPECT_THROW(castle::nbt::FromTagLongArray(data, offset), std::runtime_error);
}

// ---------------------------------------------------------------------------
// Lists
// ---------------------------------------------------------------------------

TEST(NbtDeserialization, ListEmpty) {
  // empty list is written with element type TAG_End (0) and length 0
  expectRoundTrip(makeDoc({0x09,
                           0x00,
                           0x01,
                           0x6C, // TAG_List "l"
                           0x00, // element type = TAG_End
                           0x00,
                           0x00,
                           0x00,
                           0x00}), // length 0
                  castle::nbt::FromTagList);
}

TEST(NbtDeserialization, ListOfInts) {
  expectRoundTrip(makeDoc({0x09,
                           0x00,
                           0x01,
                           0x6C,
                           0x03, // element type = TAG_Int
                           0x00,
                           0x00,
                           0x00,
                           0x02, // length 2
                           0x00,
                           0x00,
                           0x00,
                           0x01, // 1
                           0xFF,
                           0xFF,
                           0xFF,
                           0xFF}), // -1
                  castle::nbt::FromTagList);
}

TEST(NbtDeserialization, ListOfCompounds) {
  // list elements have no tag byte and no name, only the payload + TAG_End
  expectRoundTrip(makeDoc({0x09,
                           0x00,
                           0x01,
                           0x6C,
                           0x0A, // element type = TAG_Compound
                           0x00,
                           0x00,
                           0x00,
                           0x02, // length 2
                           // element 0: { a: 5b }
                           0x01,
                           0x00,
                           0x01,
                           0x61,
                           0x05,
                           0x00,
                           // element 1: {}
                           0x00}),
                  castle::nbt::FromTagList);
}

TEST(NbtDeserialization, ListOfLists) {
  expectRoundTrip(makeDoc({0x09,
                           0x00,
                           0x01,
                           0x6C,
                           0x09, // element type = TAG_List
                           0x00,
                           0x00,
                           0x00,
                           0x01, // length 1
                           // inner list: bytes [1, 2]
                           0x01,
                           0x00,
                           0x00,
                           0x00,
                           0x02,
                           0x01,
                           0x02}),
                  castle::nbt::FromTagList);
}

TEST(NbtDeserialization, ListOfStrings) {
  expectRoundTrip(makeDoc({0x09,
                           0x00,
                           0x01,
                           0x6C,
                           0x08, // element type = TAG_String
                           0x00,
                           0x00,
                           0x00,
                           0x02, // length 2
                           0x00,
                           0x02,
                           0x68,
                           0x69, // "hi"
                           0x00,
                           0x00}), // ""
                  castle::nbt::FromTagList);
}

TEST(NbtDeserialization, ListHugeLengthThrows) {
  // claims 2^31-1 ints but only has one. Must throw without reserving that much.
  const std::vector<std::uint8_t> data = {
      0x09, 0x00, 0x01, 0x6C, 0x03, 0x7F, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x01};
  std::size_t offset = 0;

  EXPECT_THROW(castle::nbt::FromTagList(data, offset), std::runtime_error);
}

// ---------------------------------------------------------------------------
// Compounds
// ---------------------------------------------------------------------------

TEST(NbtDeserialization, CompoundEmpty) {
  const std::vector<std::uint8_t> data = {0x0A, 0x00, 0x00, 0x00};
  std::size_t offset = 0;

  Nbt tag = castle::nbt::FromTagCompound(data, offset);

  EXPECT_EQ(offset, data.size());
  EXPECT_EQ(tag.tag(), NbtTag::CompoundTag);

  std::vector<std::uint8_t> result = castle::nbt::NbtWriter::writer(tag);
  EXPECT_EQ(result, data);
}

TEST(NbtDeserialization, CompoundKeepsRootName) {
  const std::vector<std::uint8_t> data = {0x0A, 0x00, 0x04, 0x72, 0x6F, 0x6F, 0x74, 0x00};
  std::size_t offset = 0;

  Nbt tag = castle::nbt::FromTagCompound(data, offset);

  EXPECT_EQ(tag.name(), "root");
  EXPECT_EQ(offset, data.size());
}

TEST(NbtDeserialization, CompoundNamelessRoot) {
  // network format (1.20.2+): tag byte, then the payload straight away
  const std::vector<std::uint8_t> data = {0x0A,
                                          0x01,
                                          0x00,
                                          0x01,
                                          0x61,
                                          0x05, // a: 5b
                                          0x00};
  std::size_t offset = 0;

  Nbt tag = castle::nbt::FromTagCompound(data, offset, TagContext::NamelessRoot);

  EXPECT_EQ(tag.name(), "");
  EXPECT_EQ(offset, data.size());
}

TEST(NbtDeserialization, CompoundNestedThenSibling) {
  // { inner: { a: 5b }, after: 7b }
  // Catches a nested compound that does not consume its own TAG_End: the parent would see that
  // 0x00 as its own end and drop "after".
  const std::vector<std::uint8_t> data = {
      0x0A, 0x00, 0x00,                                     // root ""
      0x0A, 0x00, 0x05, 0x69, 0x6E, 0x6E, 0x65, 0x72,       // compound "inner"
      0x01, 0x00, 0x01, 0x61, 0x05,                         //   byte "a" = 5
      0x00,                                                 //   end of inner
      0x01, 0x00, 0x05, 0x61, 0x66, 0x74, 0x65, 0x72, 0x07, // byte "after" = 7
      0x00                                                  // end of root
  };
  std::size_t offset = 0;

  Nbt tag = castle::nbt::FromTagCompound(data, offset);

  EXPECT_EQ(offset, data.size());

  std::vector<std::uint8_t> result = castle::nbt::NbtWriter::writer(tag);
  EXPECT_EQ(result, data);
}

TEST(NbtDeserialization, CompoundMissingEndTagThrows) {
  const std::vector<std::uint8_t> data = {0x0A, 0x00, 0x00, 0x01, 0x00, 0x01, 0x61, 0x05};
  std::size_t offset = 0;

  EXPECT_THROW(castle::nbt::FromTagCompound(data, offset), std::runtime_error);
}

TEST(NbtDeserialization, CompoundUnknownTagIdThrows) {
  // 0x0D (13) is not a defined tag type
  const std::vector<std::uint8_t> data = {0x0A, 0x00, 0x00, 0x0D, 0x00, 0x01, 0x61, 0x00};
  std::size_t offset = 0;

  EXPECT_THROW(castle::nbt::FromTagCompound(data, offset), std::runtime_error);
}

TEST(NbtDeserialization, CompoundTruncatedChildThrows) {
  // int child with only 2 of its 4 payload bytes
  const std::vector<std::uint8_t> data = {0x0A, 0x00, 0x00, 0x03, 0x00, 0x01, 0x61, 0x00, 0x01};
  std::size_t offset = 0;

  EXPECT_THROW(castle::nbt::FromTagCompound(data, offset), std::runtime_error);
}

// Enable once the depth limit exists (vanilla uses 512). Without one, this overflows the stack.
TEST(NbtDeserialization, CompoundTooDeepThrows) {
  constexpr std::size_t depth = 1000;

  std::vector<std::uint8_t> data = {0x0A, 0x00, 0x00};
  for (std::size_t i = 0; i < depth; ++i) {
    data.insert(data.end(), {0x0A, 0x00, 0x00}); // another nested compound
  }
  data.insert(data.end(), depth + 1, 0x00); // matching TAG_Ends

  std::size_t offset = 0;
  EXPECT_THROW(castle::nbt::FromTagCompound(data, offset), std::runtime_error);
}
