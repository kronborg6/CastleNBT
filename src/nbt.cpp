#include <cstddef>
#include <cstdint>
#include <limits>
#include <nbt/endian.hpp>
#include <nbt/nbt.hpp>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace minecraft::nbt {

template <typename T> T toNbtLength(size_t length) {

	static_assert(std::is_same_v<T, uint16_t> || std::is_same_v<T, int32_t>,
	              "Unsupported NBT legth type");

	if constexpr (std::is_same_v<T, uint16_t>) {
		if (length > std::numeric_limits<uint16_t>::max()) {
			throw std::runtime_error("NBT string length is too large");
		}
	}

	if constexpr (std::is_same_v<T, int32_t>) {
		if (length > std::numeric_limits<int32_t>::max()) {
			throw std::runtime_error("NBT array length is too large");
		}
	}

	return static_cast<T>(length);
}

constexpr std::uint8_t tag(NbtTag value) noexcept {
	return static_cast<uint8_t>(value);
}

template <NbtTag Tag>
constexpr void insertNameAndTag(const std::string &name,
                                std::vector<uint8_t> &data,
                                TagContext context) {
	if (context == TagContext::ListElement)
		return;

	static_assert(Tag != NbtTag::EndTag, "EndTag does not have a name");

	const std::uint16_t rawLength = toNbtLength<uint16_t>(name.length());

	data.push_back(tag(Tag));

	endian::writeBigEndian<uint16_t>(rawLength, data);

	data.insert(data.end(), name.begin(), name.end());
}

void ToEndTag(std::vector<uint8_t> &data) {
	data.push_back(tag(NbtTag::EndTag));
}

void ToTagByte(const std::string &name, int8_t value,
               std::vector<uint8_t> &data, TagContext context) {

	insertNameAndTag<NbtTag::ByteTag>(name, data, context);

	data.push_back(static_cast<uint8_t>(value));
}

void ToTagShort(const std::string &name, int16_t value,
                std::vector<std::uint8_t> &data, TagContext context) {

	insertNameAndTag<NbtTag::ShortTag>(name, data, context);

	const uint16_t rawValue = static_cast<uint16_t>(value);

	endian::writeBigEndian<uint16_t>(rawValue, data);
}

void ToTagInt(const std::string &name, int32_t value,
              std::vector<std::uint8_t> &data, TagContext context) {

	insertNameAndTag<NbtTag::IntTag>(name, data, context);

	const uint32_t rawValue = static_cast<uint32_t>(value);

	endian::writeBigEndian<uint32_t>(rawValue, data);
}

void ToTagLong(const std::string &name, int64_t value,
               std::vector<std::uint8_t> &data, TagContext context) {

	insertNameAndTag<NbtTag::LongTag>(name, data, context);

	const uint64_t rawValue = static_cast<uint64_t>(value);

	endian::writeBigEndian<uint64_t>(rawValue, data);
}

void ToTagFloat(const std::string &name, float value,
                std::vector<std::uint8_t> &data, TagContext context) {

	insertNameAndTag<NbtTag::FloatTag>(name, data, context);

	uint32_t rawFloat = endian::floatToUint32(value);

	endian::writeBigEndian<uint32_t>(rawFloat, data);
}

void ToTagDouble(const std::string &name, double value,
                 std::vector<std::uint8_t> &data, TagContext context) {

	insertNameAndTag<NbtTag::DoubleTag>(name, data, context);

	uint64_t rawFloat = endian::doubleToUint64(value);

	endian::writeBigEndian<uint64_t>(rawFloat, data);
}

void ToTagCompound(const std::string &name, std::vector<std::uint8_t> &data,
                   TagContext context) {

	insertNameAndTag<NbtTag::CompoundTag>(name, data, context);
}

void ToTagString(const std::string &name, const std::string &value,
                 std::vector<std::uint8_t> &data, TagContext context) {

	insertNameAndTag<NbtTag::StringTag>(name, data, context);

	const uint16_t rawValueLength = toNbtLength<uint16_t>(value.length());

	endian::writeBigEndian<uint16_t>(rawValueLength, data);

	data.insert(data.end(), value.begin(), value.end());
}

template <NbtTag Tag>
void ToTagList(const std::string &name, size_t listLength,
               std::vector<std::uint8_t> &data, TagContext context) {

	insertNameAndTag<NbtTag::ListTag>(name, data, context);

	const int32_t rawListLength = toNbtLength<int32_t>(listLength);

	if (listLength == 0) {
		data.push_back(tag(NbtTag::EndTag));
	} else {
		data.push_back(tag(Tag));
	}

	endian::writeBigEndian<uint32_t>(static_cast<uint32_t>(rawListLength),
	                                 data);
}

void ToTagIntArray(const std::string &name, const std::vector<int32_t> &values,
                   std::vector<std::uint8_t> &data, TagContext context) {

	insertNameAndTag<NbtTag::IntArrayTag>(name, data, context);

	const int32_t size = toNbtLength<int32_t>(values.size());

	endian::writeBigEndian<uint32_t>(static_cast<uint32_t>(size), data);

	for (int32_t value : values) {
		const uint32_t rawItem = static_cast<uint32_t>(value);
		endian::writeBigEndian<uint32_t>(rawItem, data);
	}
}

void ToTagLongArray(const std::string &name, const std::vector<int64_t> &values,
                    std::vector<std::uint8_t> &data, TagContext context) {

	insertNameAndTag<NbtTag::LongArrayTag>(name, data, context);

	const int32_t size = toNbtLength<int32_t>(values.size());

	endian::writeBigEndian<uint32_t>(static_cast<uint32_t>(size), data);

	for (int64_t value : values) {
		const uint64_t rawItem = static_cast<uint64_t>(value);
		endian::writeBigEndian<uint64_t>(rawItem, data);
	}
}

} // namespace minecraft::nbt
