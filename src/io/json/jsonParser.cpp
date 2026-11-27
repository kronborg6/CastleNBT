#include "io/json/jsonParser.hpp"
#include "castle/nbt.hpp"
#include <iostream>
#include <ostream>
#include <stdexcept>

namespace castle::nbt::io {

  void JsonParser::skipWhitespace() {
    while (_offset < _data.size()) {
      const char c = _data[_offset];

      if (c != ' ' && c != '\n' && c != '\r' && c != '\t') {
        break;
      }

      ++_offset;
    }
  }

  bool JsonParser::eof() const {
    return _offset >= _data.size();
  }

  char JsonParser::peek() const {
    if (eof()) {
      throw std::out_of_range("Unexpected end of JSON input");
    }

    return _data[_offset];
  }

  char JsonParser::consume() {
    const char c = peek();
    ++_offset;
    return c;
  }

  Nbt JsonParser::parse() {
    std::cout << _data.size() << std::endl;

    skipWhitespace();
    while (!eof()) {
      std::cout << consume() << "\n";
    }
    return Nbt::makeCompound("");
  }

  Nbt JsonParser::parseValue() {
    skipWhitespace();

    switch (peek()) {
    case '{':
      return parseObject();

    case '[':
      return parseArray();

    case '"':
      return parseString();

    case 't':
    case 'f':
      return parseBoolean();

    case 'n':
      return parseNull();

    default:
      if (peek() == '-' || (peek() >= '0' && peek() <= '9')) {
        return parseNumber();
      }

      throw std::runtime_error("Invalid JSON value");
    }
  }
} // namespace castle::nbt::io
