#include <bit>
#include <cstdint>
#include <type_traits>
#include <vector>
namespace endian {
template <typename T> void writeBigEndian(T value, std::vector<uint8_t> &data) {
	static_assert(std::is_unsigned_v<T>);

	for (int shift = static_cast<int>(sizeof(T) * 8) - 8; shift >= 0;
	     shift -= 8) {
		data.push_back(static_cast<std::uint8_t>((value >> shift) & 0xFF));
	}
}
template <typename T>
void writeLittleEndian(T value, std::vector<uint8_t> &data) {
	static_assert(std::is_unsigned_v<T>);

	for (int shift = 0; shift < static_cast<int>(sizeof(T) * 8); shift += 8) {
		data.push_back(static_cast<std::uint8_t>((value >> shift) & 0xFF));
	}
}
template void writeBigEndian<uint8_t>(uint8_t, std::vector<uint8_t> &);

template void writeBigEndian<uint16_t>(uint16_t, std::vector<uint8_t> &);

template void writeBigEndian<uint32_t>(uint32_t, std::vector<uint8_t> &);

template void writeBigEndian<uint64_t>(uint64_t, std::vector<uint8_t> &);

template void writeLittleEndian<uint8_t>(uint8_t, std::vector<uint8_t> &);

template void writeLittleEndian<uint16_t>(uint16_t, std::vector<uint8_t> &);

template void writeLittleEndian<uint32_t>(uint32_t, std::vector<uint8_t> &);

template void writeLittleEndian<uint64_t>(uint64_t, std::vector<uint8_t> &);

uint32_t floatToUint32(float value) {

	uint32_t reslut = std::bit_cast<uint32_t>(value);
	return reslut;
}
uint64_t doubleToUint64(double value) {
	uint64_t reslut = std::bit_cast<uint64_t>(value);
	return reslut;
}
} // namespace endian
