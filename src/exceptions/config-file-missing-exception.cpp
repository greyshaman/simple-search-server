#include <filesystem>
#include "config-file-missing-exception.h"

ConfigFileMissingException::ConfigFileMissingException(const std::string& in_filename)
  : ConfigParserException(
      "config file " + in_filename + " missing at " + std::filesystem::current_path().string()
    ) {}

