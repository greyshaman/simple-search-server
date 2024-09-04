#include "incorrect-version-exception.h"

IncorrectVersionException::IncorrectVersionException(const std::string& inFilename)
  : ConfigParserException(inFilename + " has incorrect version") {}
