#include "castle/lowlevel/readTag.hpp"
#include "castle/nbt.hpp"

namespace castle::nbt {

  struct ReadOptions {
      // Endian endian = Endian::Big; // Bedrock files use little endian
      bool namelessRoot = false; // network format, no root name field
      bool allowTrailingBytes = false;
      std::size_t maxDepth = 512;
  };

  Nbt readNbt(std::span<const uint8_t> data, std::size_t& offset, ReadOptions options) {
    if (data.size() < offset) {
      throw std::runtime_error("unexpected end of data");
    }

    NbtTag tag = static_cast<NbtTag>(data[offset]);

    if (tag != NbtTag::CompoundTag) {
      throw std::runtime_error("frist Tag needs to be a CompoundTag");
    }

    std::string name = readTagHeader(data, offset, NbtTag::CompoundTag);

    if (options.namelessRoot && !name.empty()) {

      throw std::runtime_error("root CompoundTag must me nameless");
    }

    return FromTagCompound(data, offset);
  }

} // namespace castle::nbt
