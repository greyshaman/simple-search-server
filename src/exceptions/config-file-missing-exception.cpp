#include <filesystem>
#include "config-file-missing-exception.h"

ConfigFileMissingException::ConfigFileMissingException(const std::string& inFilename)
  : ConfigParserException(
      "config file " + inFilename + " missing at " + std::filesystem::current_path().string()
    ) {}

