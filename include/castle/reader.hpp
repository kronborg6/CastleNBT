#pragma once

#include "nbt.hpp"
#include <cstddef>
#include <cstdint>
#include <istream>
#include <span>
namespace castle::nbt {

  struct ReadOptions {
      // Endian endian = Endian::Big; // Bedrock files use little endian
      bool namelessRoot = false; // network format, no root name field
      bool allowTrailingBytes = false;
      std::size_t maxDepth = 512;
  };

  struct NbtReader {
      static Nbt
      reader(std::span<const std::uint8_t> bytes, std::size_t& offset, ReadOptions options = {});
      static Nbt reader(std::span<const std::uint8_t> bytes, ReadOptions options = {});
      static Nbt reader(std::istream& stream, ReadOptions options = {});
  };
} // namespace castle::nbt
