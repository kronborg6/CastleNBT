#pragma once
#include "castle/nbt.hpp"
#include "castle/writer.hpp"
#include <string>
namespace castle::nbt::snbt {

  void FromTagCompoundSNBT(std::string& output, const Nbt& tag, SnbtOptions options);
  std::string snbt(const Nbt& root, SnbtOptions options);
} // namespace castle::nbt::snbt
