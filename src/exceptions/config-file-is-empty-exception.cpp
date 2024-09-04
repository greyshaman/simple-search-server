#include "config-file-is-empty-exception.h"

ConfigFileIsEmptyException::ConfigFileIsEmptyException(const std::string& inFile)
  : ConfigParserException("config file " + inFile + " is empty") {}
