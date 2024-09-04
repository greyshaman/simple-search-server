#include "no-max-responses-exception.h"

NoMaxResponsesException::NoMaxResponsesException(const std::string& inFile)
  : ConfigParserException(inFile + " should have max_responses option in config section") {}
