#include "config-parser-exception.h"

ConfigParserException::ConfigParserException(const std::string& in_message)
  : message(in_message) {}

ConfigParserException::~ConfigParserException() {}

const char* ConfigParserException::what() const noexcept {
  return message.c_str();
}
