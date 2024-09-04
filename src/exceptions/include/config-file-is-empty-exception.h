#pragma once

#include <string>
#include "config-parser-exception.h"

class ConfigFileIsEmptyException : public ConfigParserException {
public:
  ConfigFileIsEmptyException(const std::string&);
};
