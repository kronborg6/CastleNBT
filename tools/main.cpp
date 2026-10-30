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
using castle::nbt::NbtReader;
using castle::nbt::NbtWriter;
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

  Nbt root = Nbt::makeCompound("");
  root.add(Nbt::makeString("first name", "kronborg"));
  root.add(Nbt::makeString("lastName", "Mikkel"));
  root.add(Nbt::makeInt("age", 26));
  Nbt stats = Nbt::makeCompound("stats");
  stats.add(Nbt::makeInt("health", 100));
  stats.add(Nbt::makeDouble("stamina", 53));
  stats.add(Nbt::makeFloat("mana", 333));

  root.add(stats);
  Nbt lvl = Nbt::makeCompound("lvl");
  lvl.add(Nbt::makeInt("lvl", 21));
  lvl.add(Nbt::makeInt("xp", 42141));
  root.add(lvl);

  Nbt inv = Nbt::makeList("invtory");

  inv.append("Apple");
  inv.append("Pinapple");
  inv.append("Stick");
  inv.append("Stone");
  inv.append("Diamond");
  root.add(inv);

  Nbt list = Nbt::makeIntArray("numbers", {42, 1, 23, 4512, 11});

  root.add(list);
  std::vector<std::uint8_t> bytes =
      NbtWriter::writer(root, {.wrapNonCompoundRoot = false, .type = castle::nbt::SNBT});

  // printHex(bytes);

  return 0;
}
