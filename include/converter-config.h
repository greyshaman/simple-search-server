#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace search_server {
namespace converter_config {

typedef std::string ResourceFilename;

/**
 * @brief ConverterConfig class определяет структуру конфигурационного файла и используется
 * для десериализации json структуры
 */
struct ConverterConfig
{
  /**
   * @brief name имя программы
   */
  std::string name;
  /**
   * @brief version
   */
  std::string version;
  /**
   * @brief max_responses максимальное количество результатов на каждый поисковый запрос
   */
  int max_responses = 0;
  /**
   * @brief files список документов с тестами в которых производится индексация и поиск
   */
  std::vector<ResourceFilename> files;

  ConverterConfig();
};

/**
 * @brief from_json используется для десериализации из json
 */
void from_json(const json&, ConverterConfig&);

}
}
