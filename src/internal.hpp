#pragma once

#include "castle/nbt.hpp"

namespace castle::nbt {
  enum class VariableType {
    Byte,
    Short,
    Int,
    Long,
    Float,
    Double,
    ByteArray,
    String,
    List,
    Compound,
    IntArray,
    LongArray,
  };

  NbtTag tagFromVarible(const NbtValue& value);
  VariableType fromVarible(NbtValue value);

} // namespace castle::nbt
