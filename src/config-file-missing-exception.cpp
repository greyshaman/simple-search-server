#include <sstream>
#include <filesystem>
#include "config-file-missing-exception.h"

ConfigFileMissingException::ConfigFileMissingException(const std::string& inFilename) {
  std::stringstream ss;
  ss << "config file " << inFilename << " missing at " << std::filesystem::current_path();
  message = ss.str();
}

ConfigFileMissingException::~ConfigFileMissingException() {}

const char* ConfigFileMissingException::what() const noexcept {
  return message.c_str();
}
