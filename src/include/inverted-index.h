#pragma once

#include <string>
#include <vector>
#include <map>
#include <mutex>

namespace search_server {
namespace inverted_index {


struct Entry
{
  size_t doc_id, count;

  bool operator==(const Entry& other) const
  {
    return (doc_id == other.doc_id && count == other.count);
  }

  bool operator> (const Entry& other) const { return doc_id > other.doc_id; }
  bool operator< (const Entry& other) const { return doc_id < other.doc_id; }

  Entry(size_t in_doc_id, size_t in_count)
      : doc_id(in_doc_id)
      , count(in_count)
  {}
};

class InvertedIndex
{
  std::vector<std::string> docs;
  std::map<std::string, std::vector<Entry>> freqDictionary;

  static std::map<std::string, size_t> countWordsFrequencies(const std::string& document);

  static void updateFreqDictionaryForDoc(const size_t docId,
                                         InvertedIndex* idsPtr,
                                         std::mutex& accessMutex);

public:
  InvertedIndex();

  /**
   * @brief UpdateDocumentBase обновить или заполнить базу документов, по которым будем совершать поиск
   *
   * @param input_docs содержимое документов
   */
  void UpdateDocumentBase(const std::vector<std::string> input_docs);

  /**
   * @brief GetWordCount Метод считает количество вхождений слова word в загруженной базе документов
   * @param word слово, частоту вхождений которого необходимо определить
   * @return возвращает подготовленный список с частотой указанного слова
   */
  std::vector<Entry> GetWordCount(const std::string& word);
};

}
}

