#pragma once

#include "nbt.hpp"
#include <cstdint>
#include <span>
#include <string>
#include <vector>
namespace castle::nbt {

  enum CompressenLevel {
    Compressen
  };

  enum FileType {
    BIN,
    SNBT,
    nbtText
  };

  struct SnbtOptions {
      std::size_t deph = 0;
      std::size_t indentSize = 4;
      // std::uint32_t ;
  };

  struct WriteOptions {
      bool namelessRoot = false;       // network format, no root name field
      bool wrapNonCompoundRoot = true; // set to false to allow e.g. a bare StringTag root
      FileType type = BIN;
  };

  struct NbtWriter {
      static void
      writer(const Nbt& tag, std::vector<std::uint8_t>& data, WriteOptions options = {});
      static std::vector<std::uint8_t> writer(const Nbt& tag, WriteOptions options = {});
      static void writer(const Nbt& tag, const std::string& path, WriteOptions options = {});
      static std::string snbtWriter(const Nbt& tag, SnbtOptions options = {});
      static std::string snbtWriter(std::span<const uint8_t> rawNbt, SnbtOptions options = {});
      static void snbtWriter(const Nbt& tag, const std::string& path, SnbtOptions options = {});
      static void snbtWriter(std::span<const uint8_t> rawNbt,
                             const std::string& path,
                             SnbtOptions options = {});
  };
} // namespace castle::nbt
