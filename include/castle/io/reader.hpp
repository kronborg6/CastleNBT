#pragma once

#include "castle/io/format.hpp"
#include "castle/nbt.hpp"
#include <filesystem>

namespace castle::nbt::io {

  [[nodiscard]] Nbt read(const std::filesystem::path& path, Format = Format::AUTO);

  [[nodiscard]] Nbt read(std::istream& stream, Format format = Format::NBT);

  [[nodiscard]] Nbt read(std::span<const std::uint8_t> data, Format format = Format::NBT);

} // namespace castle::nbt::io
