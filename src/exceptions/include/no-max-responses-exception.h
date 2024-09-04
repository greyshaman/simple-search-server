#pragma once
#include <string>
#include "config-parser-exception.h"

class NoMaxResponsesException : public ConfigParserException {
public:
  NoMaxResponsesException(const std ::string&);
};
