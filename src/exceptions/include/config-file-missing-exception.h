#pragma once

#include <string>
#include "config-parser-exception.h"

/**
 * @brief ConfigFileMissingException позволяет диагностировать отсутствие
 * конфигурационного файла
 */
class ConfigFileMissingException : public ConfigParserException {
public:
  ConfigFileMissingException(const std::string&);
};
