#pragma once
#include <cstdint>
#include <vector>

namespace endian {
  template <typename T> void writeBigEndian(T value, std::vector<std::uint8_t>& data);
  template <typename T> void writeLittleEndian(T value, std::vector<std::uint8_t>& data);

  uint32_t floatToUint32(float value);
  uint64_t doubleToUint64(double value);
} // namespace endian
