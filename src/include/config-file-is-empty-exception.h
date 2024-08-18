#pragma once

#include <exception>

class ConfigFileIsEmptyException : public std::exception {
public:
  ConfigFileIsEmptyException() = default;

  virtual ~ConfigFileIsEmptyException();

  virtual const char* what() const noexcept;
};
