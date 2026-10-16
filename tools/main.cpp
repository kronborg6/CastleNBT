#include <castle/nbt.hpp>
#include <iomanip>
#include <iostream>
#include <vector>

void printHex(const std::vector<uint8_t>& data) {
  for (uint8_t byte : data) {
    std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << ' ';
  }

  std::cout << std::dec << '\n';
}

int main(void) {

  castle::nbt::Nbt root = castle::nbt::Nbt::makeCompound("");

  castle::nbt::Nbt name = castle::nbt::Nbt::makeString("name", "Kronborg");

  castle::nbt::Nbt stats = castle::nbt::Nbt::makeCompound("stats");

  castle::nbt::Nbt items = castle::nbt::Nbt::makeList("inventory");

  castle::nbt::Nbt itemOne = castle::nbt::Nbt::makeString("weapon", "sword");
  castle::nbt::Nbt itemTwo = castle::nbt::Nbt::makeString("weapon", "staff");
  auto itemThree = castle::nbt::makeNbt("Gun");

  std::vector<int32_t> temp = {12, 124, 11};

  items.setValue(temp);

  std::cout << "gg\n";

  // items.append(itemOne);
  // items.append(itemTwo);
  // items.append(itemThree);
  // root.add(name);
  //
  // stats.add(health);
  // stats.add(mana);
  // stats.add(xp);

  root.add(stats);
  root.add(name);
  root.add(items);

  std::vector<uint8_t> data;

  root.compile(data);

  // castle::nbt::Nbt root = castle::nbt::Nbt::makeString("hello", "kronborg");
  // castle::nbt::writeNbt(root, data, true);

  printHex(data);
  return 0;
}
