#pragma once

#include <string>
#include "config-parser-exception.h"

class IncorrectVersionException : public ConfigParserException {
public:
  IncorrectVersionException(const std::string&);
};
