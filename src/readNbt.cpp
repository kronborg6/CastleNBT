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

    TagContext ctx = options.namelessRoot ? TagContext::NamelessRoot : TagContext::Named;
    if (options.namelessRoot && options.allowNonCompoundRoot) {
      ctx = TagContext::NamelessRootAndNonCompoundRoot;
    }

    if (tag != NbtTag::CompoundTag) {
      if (options.allowNonCompoundRoot) {

        if (ctx == TagContext::NamelessRootAndNonCompoundRoot) {

          Nbt nbt = NonCompoundRoot(
              data, offset, TagContext::NamelessRootAndNonCompoundRoot, 0, options.maxDepth);

          checkTrailingBytes(options.allowTrailingBytes, data.size(), offset);

          return nbt;
        }

        Nbt nbt = NonCompoundRoot(data, offset, TagContext::NonCompoundRoot, 0, options.maxDepth);

        checkTrailingBytes(options.allowTrailingBytes, data.size(), offset);

        return nbt;
      }
      throw std::runtime_error("frist Tag needs to be a CompoundTag");
    }

    Nbt nbt = FromTagCompound(data, offset, ctx, 0, options.maxDepth);

    checkTrailingBytes(options.allowTrailingBytes, data.size(), offset);

    return nbt;
  }

} // namespace castle::nbt
