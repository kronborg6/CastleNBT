#include "castle/reader.hpp"

#include "castle/lowlevel/readTag.hpp"
#include "castle/nbt.hpp"
#include <cstddef>
#include <cstdint>
#include <istream>
#include <iterator>
#include <span>
#include <stdexcept>
namespace castle::nbt {

  Nbt NbtReader::reader(std::span<const std::uint8_t> bytes,
                        std::size_t& offset,
                        ReadOptions options) {
    return readNbt(bytes, offset);
  }
  Nbt NbtReader::reader(std::span<const std::uint8_t> bytes, ReadOptions options) {
    std::size_t offset;
    return readNbt(bytes, offset);
  }
  Nbt NbtReader::reader(std::istream& stream, ReadOptions options) {

    if (!stream) {
      throw std::runtime_error("input stream not readable");
    }

    std::vector<std::uint8_t> bytes{std::istreambuf_iterator<char>(stream), {}};

    std::size_t offset;
    return readNbt(bytes, offset);
  }
} // namespace castle::nbt
