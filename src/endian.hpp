#pragma once
#include <cstdint>
#include <span>
#include <vector>

namespace endian {
template <typename T>
void writeBigEndian(T value, std::vector<std::uint8_t> &data);
template <typename T>
void writeLittleEndian(T value, std::vector<std::uint8_t> &data);

template <std::unsigned_integral U>
U readBigEndian(const std::span<const uint8_t> data, size_t &offset);

uint32_t floatToUint32(float from);
uint64_t doubleToUint64(double from);

float uint32ToFloat(uint32_t from);
double uint64ToDouble(uint64_t from);
} // namespace endian
