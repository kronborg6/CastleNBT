#include <castle/nbt.hpp>
#include <castle/readTag.hpp>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>

using castle::nbt::Nbt;
void printHex(const std::vector<uint8_t> &data) {
  for (uint8_t byte : data) {
    std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0')
              << static_cast<int>(byte) << ' ';
  }

  std::cout << std::dec << '\n';
}

int main(void) {

  // castle::nbt::Nbt root = castle::nbt::Nbt::makeByte("kronborg", 67);
  // castle::nbt::Nbt root = castle::nbt::Nbt::makeShort("kronborg short",
  // 3123);
  castle::nbt::Nbt root =
      castle::nbt::Nbt::makeFloat("kronborg float", 312.123f);
  // castle::nbt::Nbt root = castle::nbt::Nbt::makeDouble("kronborg double",
  // 31223); castle::nbt::Nbt root = castle::nbt::Nbt::makeCompound("");

  // castle::nbt::Nbt name = castle::nbt::Nbt::makeString("name", "Kronborg");
  //
  // castle::nbt::Nbt stats = castle::nbt::Nbt::makeCompound("stats");
  //
  // castle::nbt::Nbt items = castle::nbt::Nbt::makeList("inventory");
  //
  // castle::nbt::Nbt itemOne = castle::nbt::Nbt::makeString("weapon", "sword");
  // castle::nbt::Nbt itemTwo = castle::nbt::Nbt::makeString("weapon", "staff");
  // auto itemThree = castle::nbt::makeNbt("Gun");
  //
  // std::vector<int32_t> temp = {12, 124, 11};
  //
  // items.setValue(temp);
  //
  // std::cout << "gg\n";
  //
  // // items.append(itemOne);
  // // items.append(itemTwo);
  // // items.append(itemThree);
  // // root.add(name);
  // //
  // // stats.add(health);
  // // stats.add(mana);
  // // stats.add(xp);
  //
  // root.add(stats);
  // root.add(name);
  // root.add(items);

  // root.add(castle::nbt::Nbt::makeString("hello", "world"));

  // Nbt list = Nbt::makeList("list");
  // Nbt compound1 = Nbt::makeCompound("user");
  // compound1.add(Nbt::makeString("name", "Bob"));
  // compound1.add(Nbt::makeInt("age", 31));
  //
  // Nbt compound2 = Nbt::makeCompound("user");
  // compound2.add(Nbt::makeString("name", "Laura"));
  // compound2.add(Nbt::makeInt("age", 22));
  // compound2.add(Nbt::makeInt("age", 22));
  //
  // list.append(std::move(compound1));
  // list.append(std::move(compound2));
  // // list.append(compound1);
  // // list.append(compound2);
  //
  // root.add(list);
  std::vector<uint8_t> data;

  root.compile(data);
  std::span<const std::uint8_t> ro = data;

  size_t offset = 3;
  castle::nbt::Nbt w = castle::nbt::FromTagFloat(ro, offset);

  std::cout << "name is: " << w.name() << "\n ";

  // castle::nbt::Nbt root = castle::nbt::Nbt::makeString("hello", "kronborg");
  // castle::nbt::writeNbt(root, data, true);

  printHex(data);
  return 0;
}
