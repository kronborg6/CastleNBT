#pragma once

#include "castle/nbt.hpp"
#include <string_view>
namespace castle::nbt::io {
  class JsonParser {
    public:
      explicit JsonParser(std::string_view data) : _data(data) {
      }

      [[nodiscard]] Nbt parse();

    private:
      std::string_view _data;
      std::size_t _offset = 0;

      Nbt parseValue();
      Nbt parseObject();
      Nbt parseArray();
      Nbt parseNumber();
      Nbt parseString();
      Nbt parseBoolean();
      Nbt parseNull();

      void skipWhitespace();

      char peek() const;
      char consume();
      bool eof() const;
  };
} // namespace castle::nbt::io
