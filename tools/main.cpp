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

int main(void) {

  castle::nbt::Nbt root = castle::nbt::Nbt::makeList("names");

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
  std::vector<uint8_t> data = root.compile();

  size_t offset = 3;

  // const auto& elements = std::get<std::vector<Nbt>>(list.value());
  printHex(data);
  std::span<const std::uint8_t> ro = data;
  castle::nbt::Nbt w = castle::nbt::FromTagList(ro, offset);
  printHex(w.compile());

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
