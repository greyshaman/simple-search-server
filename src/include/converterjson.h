#pragma once

#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <filesystem>

#include "converter-config.h"
#include "requests.h"

namespace search_server {

/**
 * @brief ConverterJSON - класс для работы с JSON-файлами
 */
class ConverterJSON {
  std::string config_filename;
  std::string requests_filename;
  std::string answers_filename;

  converter_config::ConverterConfig converter_config;

  requests_format::RequestsConfig requests_config;

  nlohmann::json readJsonFile(const std::string& file_name, const bool is_required) const;
  inline std::filesystem::path getParentPath(const std::string& filename) const;

  void loadConfig(const nlohmann::json&);

  requests_format::RequestsConfig loadRequests(const nlohmann::json&);
  json prepareAnswersToExport(std::vector<std::vector<std::pair<int, float>>> answer);
  void truncToNWords(std::string&, const size_t);

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
  void PutAnswers(std::vector<std::vector<std::pair<int, float>>> answer);
};
}
