#pragma once
#include "castle/lowlevel/common.hpp"
#include "castle/nbt.hpp"
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <vector>

#include <string>
#include <vector>

namespace castle::nbt {

  std::string readTagHeader(std::span<const std::uint8_t> data, std::size_t& offset, NbtTag expted);

  void ToEndTag(std::vector<uint8_t>& data);

  Nbt FromTagByte(std::span<const std::uint8_t> data,
                  std::size_t& offset,
                  nbt::TagContext context = TagContext::Named);

  Nbt FromTagShort(std::span<const std::uint8_t> data,
                   std::size_t& offset,
                   nbt::TagContext context = TagContext::Named);

  Nbt FromTagInt(std::span<const std::uint8_t> data,
                 std::size_t& offset,
                 nbt::TagContext context = TagContext::Named);

  Nbt FromTagLong(std::span<const std::uint8_t> data,
                  std::size_t& offset,
                  nbt::TagContext context = TagContext::Named);

  Nbt FromTagFloat(std::span<const std::uint8_t> data,
                   std::size_t& offset,
                   nbt::TagContext context = TagContext::Named);

  Nbt FromTagDouble(std::span<const std::uint8_t> data,
                    std::size_t& offset,
                    nbt::TagContext context = TagContext::Named);

  Nbt FromTagByteArray(std::span<const uint8_t> data,
                       std::size_t& offset,
                       nbt::TagContext context = TagContext::Named);

  Nbt FromTagString(std::span<const std::uint8_t> data,
                    std::size_t& offset,
                    nbt::TagContext context = TagContext::Named);

  Nbt FromTagList(std::span<const uint8_t> data,
                  std::size_t& offset,
                  nbt::TagContext context = TagContext::Named);

  Nbt FromTagCompound(std::span<const std::uint8_t> data,
                      size_t& offset,
                      nbt::TagContext context = TagContext::Named);

  Nbt FromTagIntArray(std::span<const uint8_t> data,
                      std::size_t& offset,
                      nbt::TagContext context = TagContext::Named);

  Nbt FromTagLongArray(std::span<const uint8_t> data,
                       std::size_t& offset,
                       nbt::TagContext context = TagContext::Named);

  namespace value {
    std::int8_t FromTagByte(std::span<const uint8_t> data,
                            std::size_t& offset,
                            nbt::TagContext context = TagContext::Named);

    std::int16_t FromTagShort(std::span<const std::uint8_t> data,
                              std::size_t& offset,
                              nbt::TagContext context = TagContext::Named);

    std::int32_t FromTagInt(std::span<const std::uint8_t> data,
                            std::size_t& offset,
                            nbt::TagContext context = TagContext::Named);

    std::int64_t FromTagLong(std::span<const std::uint8_t> data,
                             std::size_t& offset,
                             nbt::TagContext context = TagContext::Named);

    float FromTagFloat(std::span<const std::uint8_t> data,
                       std::size_t& offset,
                       nbt::TagContext context = TagContext::Named);

    double FromTagDouble(std::span<const std::uint8_t> data,
                         std::size_t& offset,
                         nbt::TagContext context = TagContext::Named);

    std::vector<std::int8_t> FromTagByteArray(const std::vector<uint8_t> data,
                                              std::size_t& offset,
                                              nbt::TagContext context = TagContext::Named);

    std::string FromTagString(std::span<const std::uint8_t>& data,
                              std::size_t& offset,
                              nbt::TagContext context = TagContext::Named);

    std::vector<castle::nbt::NbtValue> FromTagList(std::span<const uint8_t> data,
                                                   std::size_t& offset,
                                                   nbt::TagContext context = TagContext::Named);

    Nbt FromTagCompound(std::span<const std::uint8_t>& data,
                        size_t offset,
                        nbt::TagContext context = TagContext::Named);

    std::vector<int32_t> FromTagIntArray(std::span<const uint8_t> data,
                                         std::size_t& offset,
                                         nbt::TagContext context = TagContext::Named);

    std::vector<int64_t> FromTagLongArray(std::span<const uint8_t> data,
                                          std::size_t& offset,
                                          nbt::TagContext context = TagContext::Named);
  } // namespace value
} // namespace castle::nbt
