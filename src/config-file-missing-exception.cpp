#include <sstream>
#include "config-file-missing-exception.h"

ConfigFileMissingException::ConfigFileMissingException(const std::string& inFilename) {
  std::stringstream ss;
  ss << "config file " << inFilename << " missing";
  message = ss.str();
}

ConfigFileMissingException::~ConfigFileMissingException() {}

const char* ConfigFileMissingException::what() const noexcept {
  return message.c_str();
}
