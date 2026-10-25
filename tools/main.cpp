#include "castle/reader.hpp"
#include "castle/writer.hpp"
#include <castle/lowlevel/readTag.hpp>
#include <castle/nbt.hpp>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>

using castle::nbt::Nbt;
void printHex(const std::vector<uint8_t>& data) {
  for (uint8_t byte : data) {
    std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte)
              << ", ";
  }

  std::cout << std::dec << '\n';
}

namespace {

  // Root compound holding one list named "l", nested `levels` lists deep:
  // { l: [[[[ ... [] ... ]]]] }
  std::vector<std::uint8_t> nestedLists(std::size_t levels) {
    std::vector<std::uint8_t> bytes = {0x0A,
                                       0x00,
                                       0x00, // root compound, empty name
                                       0x09,
                                       0x00,
                                       0x01,
                                       'l'}; // TAG_List "l"

    for (std::size_t i = 0; i < levels; i++) {
      // this list holds 1 element, and that element is another list
      bytes.insert(bytes.end(), {0x09, 0x00, 0x00, 0x00, 0x01});
    }

    bytes.insert(bytes.end(), {0x00, 0x00, 0x00, 0x00, 0x00}); // innermost list: type End, length 0
    bytes.push_back(0x00);                                     // TAG_End for the root
    return bytes;
  }

} // namespace

int main(void) {

  // const auto bytes = castle::nbt::NbtWriter::writer(Nbt::makeString("greetinggg", "hello"),
  //                                                   {.wrapNonCompoundRoot = false});

  const auto bytes2 = castle::nbt::NbtWriter::writer(
      Nbt::makeString("", "hello"), {.namelessRoot = true, .wrapNonCompoundRoot = false});

  const std::vector<std::uint8_t> expected = {0x08, 0x00, 0x05, 'h', 'e', 'l', 'l', 'o'};

  const Nbt root = castle::nbt::NbtReader::reader(expected, {.namelessRoot = true});
  printHex(bytes2);
  printHex(expected);

  // const Nbt root =
  //     castle::nbt::NbtReader::reader(bytes); // if this throws, gtest reports the test as failed
  // printHex(castle::nbt::NbtWriter::writer(root, {.wrapNonCompoundRoot = false}));

  return 0;
}
