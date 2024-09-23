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

void InvertedIndex::updateFreqDictionaryForDoc(const size_t docId,
											   InvertedIndex* idxPtr,
											   std::mutex& accessMutex)
{
  const std::string doc = idxPtr->docs.at(docId);
  std::map<std::string, std::vector<Entry>>& dict = idxPtr->freqDictionary;
  const auto wordsFrequencies = countWordsFrequencies(doc);
  for (auto itr = wordsFrequencies.begin(); itr != wordsFrequencies.end(); itr++) {
    const auto pair = (*itr);
    const auto word = pair.first;
    const auto count = pair.second;

	std::lock_guard<std::mutex> guard{accessMutex};
	if (dict.count(word) == 0) {
	  dict[word] = std::vector<Entry>{{Entry(docId, count)}};
	} else {
	  dict[word].push_back(Entry(docId, count));
	}
  }
}

InvertedIndex::InvertedIndex()
	: docs(std::vector<std::string>())
	, freqDictionary(std::map<std::string, std::vector<Entry>>())
{}

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string> input_docs)
{
  docs.clear();
  freqDictionary.clear();


  docs.resize(input_docs.size());
  std::copy(input_docs.begin(), input_docs.end(), docs.begin());

  std::mutex accessMutex;
  std::vector<std::future<void>> workers;

  for (size_t docId = 0; docId < docs.size(); docId++) {
    const auto doc = docs[docId];
    workers.emplace_back(std::async(std::launch::async,
                                    updateFreqDictionaryForDoc,
                                    docId,
                                    this,
                                    std::ref(accessMutex)));
  }

  for (auto& worker : workers) {
    worker.wait();
  }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word)
{
  if (freqDictionary.count(word) > 0) {
    const auto foundItr = freqDictionary.find(word);
    return (*foundItr).second;
  } else {
    return std::vector<Entry>();
  }
}

}
}
