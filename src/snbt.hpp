#pragma once
#include "castle/nbt.hpp"
#include "castle/writer.hpp"
#include <cstddef>
#include <cstdint>
#include <string>
namespace castle::nbt::snbt {
  struct Options {
      WriteOptions options;
      std::size_t deph = 0;
      std::size_t indentSize = 4;
      // std::uint32_t ;
  };

  void FromTagCompoundSNBT(std::string& output, const Nbt& tag, Options options);
  std::string snbt(const Nbt& root, WriteOptions options);
} // namespace castle::nbt::snbt
