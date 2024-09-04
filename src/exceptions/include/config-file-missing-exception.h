#pragma once

#include <string>
#include "config-parser-exception.h"

class ConfigFileMissingException : public ConfigParserException {
public:
  ConfigFileMissingException(const std::string&);
};
