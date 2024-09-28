#include "exceptions/config-file-is-empty-exception.h"

ConfigFileIsEmptyException::ConfigFileIsEmptyException(const std::string& in_file)
  : ConfigParserException("config file " + in_file + " is empty") {}
