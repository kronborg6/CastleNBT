#pragma once

#include "nbt.hpp"
#include <cstddef>
#include <cstdint>
#include <istream>
#include <span>
#include <string>
#include <string_view>
#include <vector>
namespace castle::nbt {

  struct ReadOptions {
      // Endian endian = Endian::Big; // Bedrock files use little endian
      bool namelessRoot = false; // network format, no root name field
      bool allowTrailingBytes = false;
      bool allowNonCompoundRoot = true;
      std::size_t maxDepth = 512;
  };

  struct NbtReader {
      static Nbt
      reader(std::span<const std::uint8_t> bytes, std::size_t& offset, ReadOptions options = {});
      static Nbt reader(std::span<const std::uint8_t> bytes, ReadOptions options = {});
      static Nbt reader(std::istream& stream, ReadOptions options = {});
      static Nbt reader(const std::string& path, ReadOptions options = {});
      static size_t
      reader(const std::string& path, std::vector<uint8_t>& output, ReadOptions options = {});
  };
} // namespace castle::nbt
