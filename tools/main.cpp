#include "../src/io/json/jsonParser.hpp"
#include "castle/reader.hpp"
#include "castle/writer.hpp"
#include <castle/lowlevel/readTag.hpp>
#include <castle/nbt.hpp>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
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

int main(void) {

  Nbt root = Nbt::makeCompound("");
  root.add(Nbt::makeString("first name", "kronborg aka \"mikkel\""));
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
  /* auto snbt = */ NbtWriter::snbtWriter(root, "temp/player.snbt");

  const std::filesystem::path path = "./temp/helloWorld.json";

  std::cout << "Working directory: " << std::filesystem::current_path() << '\n';

  std::cout << "Absolute path: " << std::filesystem::absolute(path) << '\n';

  std::cout << "File exists: " << std::filesystem::exists(path) << '\n';

  std::ifstream stream(path);

  if (!stream.is_open()) {
    throw std::runtime_error("Failed to open JSON file");
  }

  std::string data{std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};

  std::cout << "Bytes read: " << data.size() << '\n';
  castle::nbt::io::JsonParser p(data);
  auto temp = p.parse();

  // std::cout << snbt << "\n";

  // printHex(bytes);

  return 0;
}
