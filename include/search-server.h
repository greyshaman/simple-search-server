#pragma once

#include <string>
#include <vector>
#include <cfloat>

#include "inverted-index.h"
#include "export.h"

using namespace std;

namespace search_server {

const int MAX_RESPONSES_DEFAULT = 3;
struct RelativeRelevance
{
  size_t doc_id;
  float rank;

  bool operator== (const RelativeRelevance& other) const
  {
    return (doc_id == other.doc_id && (rank - other.rank) < FLT_EPSILON);
  }
};

/**
 * @brief compareByWordEnries функциональный объект который используется для сравнения
 * векторов содержащих информациию о вхождении слова в текст документов
 */
struct compareByWordEnries
{
  bool operator()(const vector<inverted_index::Entry>& left,
                  const vector<inverted_index::Entry>& right) const
  {
    return entriesCountSum(left) < entriesCountSum(right);
  }

private:
  size_t entriesCountSum(const vector<inverted_index::Entry>& entries_list) const
  {
    size_t result = 0;
    for (auto& entry : entries_list) {
      result += entry.count;
    }
    return result;
  }
};

/**
 * @brief AbsRelevance структура регламентирующая абсолютную релевантность
 */
struct AbsRelevance
{
  size_t doc_id, relevance;

  bool operator==(const AbsRelevance& other) const
  {
    return (doc_id == other.doc_id && relevance == other.relevance);
  }
};

/**
 * @brief The SearchServer class позволяет определять наиболее
 *        релевантные, соответствующие поисковому запросу документы по прочитанным из
 *        файла requests.json поисковым запросам
 */
class SearchServer
{
  inverted_index::InvertedIndex index;

  vector<AbsRelevance> calcAbsRelevances(
      const vector<vector<inverted_index::Entry>>& freq_matrix) const;

public:
  /**
   * @brief SearchServer конструктор создающий экземпляр SearchServer
   * @param idx в конструктор класса передаётся ссылка на класс InvertedIndex,
   *        чтобы SearchServer мог узнать частоту слов встречаемых в запросе
   */
  SEARCHENGINE_EXPORT SearchServer(inverted_index::InvertedIndex& idx)
      : index(idx) {};

  /**
   * @brief search метод обработки поисковых запросов
   * @param queries_input поисковые запросы взятые из requests.json
   * @param max_reponses максимальное количество результатов выводимых для каждого запроса
   * @return возвращает отсортированный список релевантных ответов для заданных запросов
   */
  SEARCHENGINE_EXPORT vector<vector<RelativeRelevance>> search(const vector<std::string>& queries_input,
                                           const int max_responses = MAX_RESPONSES_DEFAULT);
};

} // namespace search_server
