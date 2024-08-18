#include "config-file-is-empty-exception.h"

ConfigFileIsEmptyException::~ConfigFileIsEmptyException() {}

const char* ConfigFileIsEmptyException::what() const noexcept {
  return "config file is empty";
}
