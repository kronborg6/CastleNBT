#pragma once

#include "castle/io/format.hpp"
#include "castle/nbt.hpp"
#include <filesystem>
#include <ostream>
#include <string>

namespace castle::nbt::io {
  struct WriteOptions {
      Format format = Format::Auto;
      bool prettyPrint = true;
  };

  // Write to a file
  void write(const std::filesystem::path& path, const Nbt& data, WriteOptions options = {});

  // Write to a stream
  void write(std::ostream& stream, const Nbt& data, WriteOptions options);

  // Write to a byte vector
  void write(std::vector<std::uint8_t>& output, const Nbt& data, WriteOptions options);

  // Convert to a string
  [[nodiscard]] std::string writeString(const Nbt& data, WriteOptions options);

} // namespace castle::nbt::io
