#include <bit>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <type_traits>
#include <vector>
namespace endian {
  template <typename T> void writeBigEndian(T value, std::vector<uint8_t>& data) {
    static_assert(std::is_unsigned_v<T>);

    for (int shift = static_cast<int>(sizeof(T) * 8) - 8; shift >= 0; shift -= 8) {
      data.push_back(static_cast<std::uint8_t>((value >> shift) & 0xFF));
    }
  }
  template <typename T> void writeLittleEndian(T value, std::vector<uint8_t>& data) {
    static_assert(std::is_unsigned_v<T>);

    for (int shift = 0; shift < static_cast<int>(sizeof(T) * 8); shift += 8) {
      data.push_back(static_cast<std::uint8_t>((value >> shift) & 0xFF));
    }
  }

  template <std::unsigned_integral U>
  U readBigEndian(std::span<const std::uint8_t> data, size_t& offset) {
    if (offset > data.size() || data.size() - offset < sizeof(U)) {
      throw std::runtime_error("readBigEndian: unexpected end of data");
    }

    std::size_t size = sizeof(U);

    U value = 0;

    for (std::size_t i = 0; i < size; i++) {
      value = static_cast<U>((value << 8) | data[offset + i]);
    }

    offset += size;

    return value;
  }
  template void writeBigEndian<uint8_t>(uint8_t, std::vector<uint8_t>&);

  template void writeBigEndian<uint16_t>(uint16_t, std::vector<uint8_t>&);

  template void writeBigEndian<uint32_t>(uint32_t, std::vector<uint8_t>&);

  template void writeBigEndian<uint64_t>(uint64_t, std::vector<uint8_t>&);

  template void writeLittleEndian<uint8_t>(uint8_t, std::vector<uint8_t>&);

  template void writeLittleEndian<uint16_t>(uint16_t, std::vector<uint8_t>&);

  template void writeLittleEndian<uint32_t>(uint32_t, std::vector<uint8_t>&);

  template void writeLittleEndian<uint64_t>(uint64_t, std::vector<uint8_t>&);

  template uint8_t readBigEndian<uint8_t>(std::span<const std::uint8_t>, std::size_t&);
  template uint16_t readBigEndian<uint16_t>(std::span<const std::uint8_t>, std::size_t&);
  template uint32_t readBigEndian<uint32_t>(std::span<const std::uint8_t>, std::size_t&);
  template uint64_t readBigEndian<uint64_t>(std::span<const std::uint8_t>, std::size_t&);

  uint32_t floatToUint32(float value) {

    uint32_t reslut = std::bit_cast<uint32_t>(value);
    return reslut;
  }
  uint64_t doubleToUint64(double value) {
    uint64_t reslut = std::bit_cast<uint64_t>(value);
    return reslut;
  }
} // namespace endian
