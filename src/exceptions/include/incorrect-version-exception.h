#pragma once

#include <string>
#include "config-parser-exception.h"

/**
 * @brief IncorrectVersionException позволяет диагностировать некорректность
 * указанной версии в конфигурационном файле
 */
class IncorrectVersionException : public ConfigParserException {
public:
  IncorrectVersionException(const std::string&);
};
