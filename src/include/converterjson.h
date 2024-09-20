#pragma once

#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <filesystem>

#include "converter-config.h"

/**
 * @brief ConverterJSON - класс для работы с JSON-файлами
 */
class ConverterJSON {
  std::string configFilename;
  std::string requestsFilename;
  std::string answersFilename;

  ConverterConfig converterConfig;

  std::vector<std::string> requirests;

  nlohmann::json readJsonFile(const std::string& fileName, const bool isRequired);
  std::filesystem::path getParentPath(const std::string& fileName) const;

  void loadConfig(const nlohmann::json);
  std::string checkRequiredParameter(const std::string, const std::string);
  int checkRequiredParameter(const std::string, const int);

  std::vector<std::string> loadRequests(const nlohmann::json);

public:
  ConverterJSON();
  ConverterJSON(std::string, std::string, std::string);
  ~ConverterJSON();

  /**
   * @brief GetTextDocuments - метод получения содержимого
   * файлов
   * @return std::vector<std::string> Возвращает список с содержимым файлов
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
