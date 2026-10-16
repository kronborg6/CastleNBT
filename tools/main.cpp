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
  // castle::nbt::Nbt{.tag = castle::nbt::NbtTag::StringTag, .name = "name", .value = "Kronborg"};

  castle::nbt::Nbt stats = castle::nbt::Nbt::makeCompound("stats");
  // castle::nbt::Nbt health = {
  //     .tag = castle::nbt::NbtTag::FloatTag, .name = "health", .value = 100.0f};
  // castle::nbt::Nbt mana = {.tag = castle::nbt::NbtTag::IntTag, .name = "mana", .value = 420};
  // castle::nbt::Nbt xp = {.tag = castle::nbt::NbtTag::IntTag, .name = "xp", .value = 1231};
  // castle::nbt::Nbt items = {.tag = castle::nbt::NbtTag::ListTag,
  //                           .name = "inventory",
  //                           .value = std::vector<castle::nbt::Nbt>{}};

  castle::nbt::Nbt itemOne = castle::nbt::Nbt::makeString("weapon", "sword");
  castle::nbt::Nbt itemTwo = castle::nbt::Nbt::makeString("weapon", "staff");

  // items.append(itemOne);
  // items.append(itemTwo);
  // root.add(name);
  //
  // stats.add(health);
  // stats.add(mana);
  // stats.add(xp);

  root.add(stats);
  root.add(name);
  // root.add(items);

  std::vector<uint8_t> data;

  castle::nbt::writeNbt(root, data);

  printHex(data);
  return 0;
}
