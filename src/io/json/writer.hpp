#pragma once

#include "castle/nbt.hpp"
#include <string>
namespace castle::nbt::io {
  template <typename T> T write(const Nbt& tag, /*options*/);

} // namespace castle::nbt::io
