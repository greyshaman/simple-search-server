#pragma once

#include <exception>
#include <string>

class ConfigFileMissingException : public std::exception {
public:
  ConfigFileMissingException(const std::string&);

  virtual ~ConfigFileMissingException();

  virtual const char* what() const noexcept;

protected:
  std::string message;
};
