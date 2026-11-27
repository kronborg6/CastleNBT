#pragma once

#include "castle/nbt.hpp"
#include <cstdint>
#include <vector>
namespace castle::nbt::io {
  std::vector<std::uint8_t> write(const Nbt& tag /*, options*/);

} // namespace castle::nbt::io
