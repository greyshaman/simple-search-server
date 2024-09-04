#pragma once
#include "config-parser-exception.h"

class FilesSectionMissingException : public ConfigParserException {
public:
  FilesSectionMissingException(const std::string&);
};
