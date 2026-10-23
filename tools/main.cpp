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

  auto root = Nbt::makeCompound("");
  root.add(Nbt::makeString("hello", "world"));

  std::vector<std::uint8_t> result = castle::nbt::NbtWriter::writer(root);
  // castle::nbt::NbtReader::reader(nestedLists(2000), {.maxDepth = 3000});
  // constexpr std::size_t depth = 1000;
  //
  // std::vector<std::uint8_t> data = {0x0A, 0x00, 0x00};
  // for (std::size_t i = 0; i < depth; ++i) {
  //   data.insert(data.end(), {0x0A, 0x00, 0x00}); // another nested compound
  // }
  // data.insert(data.end(), depth + 1, 0x00); // matching TAG_Ends
  //
  // std::size_t offset = 0;
  // castle::nbt::FromTagCompound(data, offset);
  // std::vector<std::uint8_t> expted = {
  //     0x0a, // Root compound
  //     0x00, // Frist child tag
  //     0x00, // Name length
  //     0x00  // End compound
  // };
  // // clang-format on
  // Nbt root = castle::nbt::Nbt::makeCompound("");
  //
  // std::vector<std::uint8_t> result = castle::nbt::NbtWriter::writer(root);

  // printHex(data);

  // castle::nbt::Nbt root = castle::nbt::Nbt::makeList("names");

  // root.append(castle::nbt::Nbt::makeString("", "Mikkel"));
  // root.append(castle::nbt::Nbt::makeString("", "Tina"));
  // root.append(castle::nbt::Nbt::makeString("", "Kristian"));
  // root.append(castle::nbt::Nbt::makeString("", "kronborg"));
  // root.add(castle::nbt::Nbt::makeInt("age", 26));
  // castle::nbt::Nbt stats = castle::nbt::Nbt::makeCompound("stats");
  // stats.add(castle::nbt::Nbt::makeInt("health", 26));
  // stats.add(castle::nbt::Nbt::makeFloat("mana", 26.04f));
  // stats.add(castle::nbt::Nbt::makeInt("xp", 26));
  // root.add(stats);
  // std::vector<uint8_t> data = root.compile();

  // size_t offset = 3;

  // const auto& elements = std::get<std::vector<Nbt>>(list.value());
  // printHex(data);
  // std::span<const std::uint8_t> ro = data;
  // castle::nbt::Nbt w = castle::nbt::FromTagList(ro, offset);
  // printHex(w.compile());

  // for (castle::nbt::Nbt ta : root.get<castle::nbt::NbtTag::CompoundTag>()) {
  //   std::cout << "name: " << ta.name() << "\n";
  //   if (ta.tag() == castle::nbt::NbtTag::CompoundTag) {
  //     for (castle::nbt::Nbt taIner : ta.get<castle::nbt::NbtTag::CompoundTag>()) {
  //       std::cout << "chiled name: " << taIner.name() << "\n";
  //     }
  //   }
  // }
  // std::cout << "name is: " << w.name() << "\n ";

  // castle::nbt::Nbt root = castle::nbt::Nbt::makeString("hello", "kronborg");
  // castle::nbt::writeNbt(root, data, true);

  return 0;
}
// compound check

// int main(void) {
//
//   castle::nbt::Nbt root = castle::nbt::Nbt::makeCompound("g");
//
//   root.add(castle::nbt::Nbt::makeString("name", "kronborg"));
//   root.add(castle::nbt::Nbt::makeInt("age", 26));
//   castle::nbt::Nbt stats = castle::nbt::Nbt::makeCompound("stats");
//   stats.add(castle::nbt::Nbt::makeInt("health", 26));
//   stats.add(castle::nbt::Nbt::makeFloat("mana", 26.04f));
//   stats.add(castle::nbt::Nbt::makeInt("xp", 26));
//   root.add(stats);
//   std::vector<uint8_t> data = root.compile();
//
//   size_t offset = 0;
//
//   // const auto& elements = std::get<std::vector<Nbt>>(list.value());
//   printHex(data);
//   std::span<const std::uint8_t> ro = data;
//   castle::nbt::Nbt w = castle::nbt::FromTagCompound(ro, offset);
//   printHex(w.compile());
//
//   for (castle::nbt::Nbt ta : root.get<castle::nbt::NbtTag::CompoundTag>()) {
//     std::cout << "name: " << ta.name() << "\n";
//     if (ta.tag() == castle::nbt::NbtTag::CompoundTag) {
//       for (castle::nbt::Nbt taIner : ta.get<castle::nbt::NbtTag::CompoundTag>()) {
//         std::cout << "chiled name: " << taIner.name() << "\n";
//       }
//     }
//   }
//   // std::cout << "name is: " << w.name() << "\n ";
//
//   // castle::nbt::Nbt root = castle::nbt::Nbt::makeString("hello", "kronborg");
//   // castle::nbt::writeNbt(root, data, true);
//
//   return 0;
// }
