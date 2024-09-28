#pragma once
#include <string>
#include "config-parser-exception.h"

/**
 * @brief NoMaxResponsesException позволяет диагностировать остутствие
 * max_reponses параметра в конфигурационном файле
 */
class NoMaxResponsesException : public ConfigParserException {
public:
  NoMaxResponsesException(const std ::string&);
};
