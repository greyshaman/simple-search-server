#include "config-parser-exception.h"

ConfigParserException::ConfigParserException(const std::string& inMessage)
  : message(inMessage) {}

ConfigParserException::~ConfigParserException() {}

const char* ConfigParserException::what() const noexcept {
  return message.c_str();
}
