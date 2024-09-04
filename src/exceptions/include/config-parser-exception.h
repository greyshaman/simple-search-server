#pragma once
#include <exception>
#include <string>

class ConfigParserException : public std::exception {
  std::string message;

public:
  ConfigParserException(const std::string&);
  virtual ~ConfigParserException();

  virtual const char* what() const noexcept;
};
