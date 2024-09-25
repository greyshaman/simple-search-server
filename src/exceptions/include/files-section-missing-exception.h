#pragma once
#include "config-parser-exception.h"

/**
 * @brief FilesSectionMissingException позволяет диагностировать отстутствие
 * секции files в конфигурационном файле
 */
class FilesSectionMissingException : public ConfigParserException {
public:
  FilesSectionMissingException(const std::string&);
};
