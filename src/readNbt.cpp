#include "castle/lowlevel/common.hpp"
#include "castle/lowlevel/readTag.hpp"
#include "castle/nbt.hpp"
#include <cstddef>
#include <stdexcept>

namespace {
  void checkTrailingBytes(bool allow, const std::size_t& dataLength, const std::size_t& offset) {
    if (!allow && dataLength > offset) {
      throw std::runtime_error("Trailing Bytes is not allowed");
    }
  }
} // namespace

namespace castle::nbt {

  Nbt readNbt(std::span<const uint8_t> data, std::size_t& offset, ReadOptions options) {
    if (data.size() <= offset) {
      throw std::runtime_error("unexpected end of data");
    }

    NbtTag tag = static_cast<NbtTag>(data[offset]);

    if (tag != NbtTag::CompoundTag) {
      throw std::runtime_error("frist Tag needs to be a CompoundTag");
    }

    TagContext ctx = options.namelessRoot ? TagContext::NamelessRoot : TagContext::Named;

    Nbt nbt = FromTagCompound(data, offset, ctx, 0, options.maxDepth);

    checkTrailingBytes(options.allowTrailingBytes, data.size(), offset);

    return nbt;
  }

} // namespace castle::nbt
