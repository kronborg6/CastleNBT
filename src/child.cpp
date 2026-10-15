#include "castle/nbt.hpp"
#include <algorithm>
#include <stdexcept>
#include <vector>

namespace castle::nbt {
  void Nbt::add(Nbt child) {
    if (_tag != NbtTag::CompoundTag) {
      throw std::runtime_error("add() requires a compound");
    }

    auto* values = std::get_if<std::vector<Nbt>>(&_value);

    if (values == nullptr) {
      throw std::runtime_error("invalid container value");
    }

    values->push_back(std::move(child));
  }

  void Nbt::append(Nbt child) {
    if (_tag != NbtTag::ListTag) {
      throw std::runtime_error("add() requires a list");
    }

    auto* values = std::get_if<std::vector<Nbt>>(&_value);

    if (values == nullptr) {
      throw std::runtime_error("invalid container value");
    }

    if (_tag == NbtTag::ListTag && !values->empty() && child.tag() != values->front().tag()) {
      throw std::runtime_error("all list elements must have the same tag");
    }

    values->push_back(std::move(child));
  }
} // namespace castle::nbt
