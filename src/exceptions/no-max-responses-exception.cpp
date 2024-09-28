#include "exceptions/no-max-responses-exception.h"

NoMaxResponsesException::NoMaxResponsesException(const std::string& in_file)
  : ConfigParserException(in_file + " should have max_responses option in config section") {}
