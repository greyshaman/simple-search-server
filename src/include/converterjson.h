#pragma once

#include <vector>
#include <string>

#include "converter-config.h"

/**
 * @brief ConverterJSON - класс для работы с JSON-файлами
 */
class ConverterJSON {
  std::string configFilename;
  std::string requestsFilename;
  std::string answersFilename;

  ConverterConfig converterConfig;

  void readConfig();
  void loadConfig();

public:
  ConverterJSON();
  ConverterJSON(std::string, std::string, std::string);
  ~ConverterJSON();

  /**
   * @brief GetTextDocuments - метод получения содержимого
   * файлов
   * @return std::vector<std::string> Возвращает список с содержиыи файлов
   *        перечисленных в config.json
   */
  std::vector<std::string> GetTextDocuments();

  /**
   * @brief GetResponsesLimit - метод считывает поле max_responses для определения
   *        предельного количества ответов на один запрос
   * @return int
   */
  int GetResponsesLimit();

  /**
   * @brief GetRequests - метод для получения запросов из файла requests.json
   * @return std::vector<std:string> возвращает список запросов из файла
   */
  std::vector<std::string> GetRequests();

  /**
   * @brief putAnswers - Положить в файл answer.json результаты поисковых запросов
   * @param std::vector<std::vector<std::pair<int, float>>> answer
   */
  void putAnswers(std::vector<std::vector<std::pair<int, float>>> answer);
};
