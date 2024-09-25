#pragma once
#include <exception>
#include <string>

/**
 * @brief ConfigParserException базовый класс для исключений связанных
 * с процессом распознования конфигорационного файлв
 */
class ConfigParserException : public std::exception {
  std::string message;

public:
  /**
   * @brief ConfigParserException конструктор
   * @param имя конфигурационного файла
   */
  ConfigParserException(const std::string&);
  virtual ~ConfigParserException();

  virtual const char* what() const noexcept;
};
