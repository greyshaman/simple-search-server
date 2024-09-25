#include <sstream>
#include <algorithm>
#include <future>

#include "inverted-index.h"

namespace search_server {
namespace inverted_index {

std::map<std::string, size_t> InvertedIndex::countWordsFrequencies(const std::string& document)
{
  std::map<std::string, size_t> result;
  std::stringstream ss(document);
  std::string word;
  while (ss >> word) {
    ++result[word];
  }
  return result;
}

void InvertedIndex::updateFreqDictionaryForDoc(const size_t doc_id,
											   InvertedIndex* idx_ptr,
											   std::mutex& access_mutex)
{
  const std::string doc = idx_ptr->docs.at(doc_id);
  std::map<std::string, std::vector<Entry>>& dict = idx_ptr->freq_dictionary;
  const auto words_frequencies = countWordsFrequencies(doc);
  for (auto itr = words_frequencies.begin(); itr != words_frequencies.end(); itr++) {
    const auto pair = (*itr);
    const auto word = pair.first;
    const auto count = pair.second;

	std::lock_guard<std::mutex> guard{access_mutex};
	if (dict.count(word) == 0) {
	  dict[word] = std::vector<Entry>{{Entry(doc_id, count)}};
	} else {
	  dict[word].emplace_back(Entry(doc_id, count));
	}
  }
}

InvertedIndex::InvertedIndex()
	: docs(std::vector<std::string>())
	, freq_dictionary(std::map<std::string, std::vector<Entry>>())
{}

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string> input_docs)
{
  docs.clear();
  freq_dictionary.clear();


  docs.resize(input_docs.size());
  std::copy(input_docs.begin(), input_docs.end(), docs.begin());

  std::mutex access_mutex;
  std::vector<std::future<void>> workers;

  for (size_t doc_id = 0; doc_id < docs.size(); doc_id++) {
    const auto doc = docs[doc_id];
    workers.emplace_back(std::async(std::launch::async,
                                    updateFreqDictionaryForDoc,
                                    doc_id,
                                    this,
                                    std::ref(access_mutex)));
  }

  for (auto& worker : workers) {
    worker.wait();
  }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) const
{
  std::vector<Entry> result;
  if (freq_dictionary.count(word) > 0) {
    const auto found_itr = freq_dictionary.find(word);
    const auto word_entries = (*found_itr).second;
    result.resize(word_entries.size());
    std::copy(word_entries.begin(), word_entries.end(), result.begin());
  }
  return result;
}

}
}
