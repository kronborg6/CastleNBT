#include "castle/lowlevel/common.hpp"
#include "castle/lowlevel/readTag.hpp"
#include "castle/nbt.hpp"

namespace castle::nbt {

  Nbt readNbt(std::span<const uint8_t> data, std::size_t& offset, ReadOptions options) {
    if (data.size() <= offset) {
      throw std::runtime_error("unexpected end of data");
    }

    NbtTag tag = static_cast<NbtTag>(data[offset]);

    if (tag != NbtTag::CompoundTag) {
      throw std::runtime_error("frist Tag needs to be a CompoundTag");
    }

    if (options.namelessRoot) {
      return FromTagCompound(data, offset, TagContext::NamelessRoot);
    }

    return FromTagCompound(data, offset);
  }

} // namespace castle::nbt
