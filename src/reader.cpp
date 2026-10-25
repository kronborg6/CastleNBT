#include "castle/reader.hpp"

#include "castle/lowlevel/readTag.hpp"
#include "castle/nbt.hpp"
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <istream>
#include <iterator>
#include <span>
#include <stdexcept>
#include <vector>
namespace {
  void printHex(const std::vector<std::uint8_t>& data) {
    for (const auto byte : data) {
      std::cout << std::format("{:02x} ", byte);
    }

    std::cout << '\n';
  }
} // namespace
namespace castle::nbt {

  Nbt NbtReader::reader(std::span<const std::uint8_t> bytes,
                        std::size_t& offset,
                        ReadOptions options) {
    return readNbt(bytes, offset, options);
  }
  Nbt NbtReader::reader(std::span<const std::uint8_t> bytes, ReadOptions options) {
    std::size_t offset = 0;
    return readNbt(bytes, offset, options);
  }
  Nbt NbtReader::reader(std::istream& stream, ReadOptions options) {

    if (!stream) {
      throw std::runtime_error("input stream not readable");
    }

    std::vector<std::uint8_t> bytes{std::istreambuf_iterator<char>(stream), {}};

    std::size_t offset = 0;
    return readNbt(bytes, offset, options);
  }
  Nbt NbtReader::reader(const std::string& path, ReadOptions options) {
    std::ifstream file(path, std::ios::binary);

    if (!file) {
      throw std::runtime_error("Failed to open file: " + std::filesystem::absolute(path).string());
    }

    auto size = std::filesystem::file_size(path);

    std::cout << "File size: " << size << " bytes\n";

    std::vector<std::uint8_t> data{std::istreambuf_iterator<char>(file),
                                   std::istreambuf_iterator<char>()};

    std::size_t offset = 0;
    printHex(data);
    return readNbt(data, offset, options);
  }
} // namespace castle::nbt
