#pragma once

#include <string>
#include "config-parser-exception.h"

/**
 * @brief ConfigFileIsEmptyException позволяет диагностировать случаи
 * с пустым конфигурационным файлом
 */
class ConfigFileIsEmptyException : public ConfigParserException {
public:
  ConfigFileIsEmptyException(const std::string&);
};
