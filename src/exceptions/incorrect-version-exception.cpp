#include "incorrect-version-exception.h"

IncorrectVersionException::IncorrectVersionException(const std::string& in_filename)
  : ConfigParserException(in_filename + " has incorrect version") {}
