#pragma once

#include "nbt.hpp"
#include <cstdint>
#include <vector>
namespace castle::nbt {

  enum CompressenLevel {
    Compressen
  };

  struct WriteOptions {
      bool namelessRoot = false;       // network format, no root name field
      bool wrapNonCompoundRoot = true; // set to false to allow e.g. a bare StringTag root
  };

  struct NbtWriter {
      static void
      writer(const Nbt& tag, std::vector<std::uint8_t>& data, WriteOptions options = {});
      static std::vector<std::uint8_t> writer(const Nbt& tag, WriteOptions options = {});
      static void writer(const Nbt& tag, const std::string& path, WriteOptions options = {});
  };
} // namespace castle::nbt
