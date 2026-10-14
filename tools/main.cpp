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

  castle::nbt::Nbt nbt{
      .tag = castle::nbt::NbtTag::CompoundTag,
      .context = castle::nbt::TagContext::Named,
      .name = "",
      .value = std::vector<castle::nbt::Nbt>{
          castle::nbt::Nbt{.tag = castle::nbt::NbtTag::StringTag,
                           .context = castle::nbt::TagContext::Named,
                           .name = "name",
                           .value = "Kronborg"},
          castle::nbt::Nbt{.tag = castle::nbt::NbtTag::CompoundTag,
                           .context = castle::nbt::TagContext::Named,
                           .name = "stats",
                           .value = std::vector<castle::nbt::Nbt>{
                               castle::nbt::Nbt{.tag = castle::nbt::NbtTag::FloatTag,
                                                .context = castle::nbt::TagContext::Named,
                                                .name = "health",
                                                .value = 100.0f},
                               castle::nbt::Nbt{.tag = castle::nbt::NbtTag::IntTag,
                                                .context = castle::nbt::TagContext::Named,
                                                .name = "mana",
                                                .value = 420},
                               castle::nbt::Nbt{.tag = castle::nbt::NbtTag::IntTag,
                                                .context = castle::nbt::TagContext::Named,
                                                .name = "xp",
                                                .value = 1231}}}}};

  std::vector<uint8_t> data;

  castle::nbt::writeNbt(nbt, data);

  printHex(data);
  return 0;
}
