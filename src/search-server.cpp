#include <algorithm>
#include <map>
#include <numeric>
#include <sstream>
#include <unordered_set>
#include <cfloat>

#include "search-server.h"

using namespace std;
namespace search_server {

vector<AbsRelevance> SearchServer::calcAbsRelevances(
    const vector<vector<inverted_index::Entry>>& freq_matrix) const
{
  vector<AbsRelevance> result;
  map<size_t, size_t> entries_by_doc_id;
  for (const auto& word_entries : freq_matrix) {
    for (const auto& entry : word_entries) {
      entries_by_doc_id[entry.doc_id] += entry.count;
    }
  }

  for (auto pair : entries_by_doc_id) {
    result.emplace_back(AbsRelevance{pair.first, pair.second});
  }
  return result;
}

vector<vector<RelativeRelevance>> SearchServer::search(const vector<std::string>& queries_input,
                                                       const int max_responses)
{
  vector<vector<RelativeRelevance>> search_result;
  for (auto& query : queries_input) {
    stringstream ss(query);
    unordered_set<string> uniq_query_words;
    {
      string word;
      while (ss >> word) {
        uniq_query_words.insert(word);
      }
    }
    vector<vector<inverted_index::Entry>> freq_matrix;
    // выбрать записи из index которые соответствуют словам из uniq_query_words и записать результат в freq_matrix
    for (auto word : uniq_query_words) {
      vector<inverted_index::Entry> word_entries = index.GetWordCount(word);
      freq_matrix.push_back(word_entries);
    }

	// Сортировать список freq_matrix по возрастанию суммарной частоты
	sort(freq_matrix.begin(), freq_matrix.end(), compareByWordEnries());

	// Для каждого документа из freq_matrix вычислить абсолютную релевантность как сумму rank по разным словам
	vector<AbsRelevance> abs_relevances = calcAbsRelevances(freq_matrix);

	// Отсортировать список abs_relevances по убыванию релевантности или если редевантности
	// равны то по возрастанию doc_id
	sort(abs_relevances.begin(),
		 abs_relevances.end(),
		 [](const AbsRelevance& left, const AbsRelevance& right) {
		   if (left.relevance == right.relevance) {
			 return left.doc_id < right.doc_id;
		   }
		   return left.relevance > right.relevance;
		 }
		);

	// принять релевантность из первого элемента отсортированного abs_relevances как максимальное
	size_t max_abs_relevance = abs_relevances.size() > 0 ? abs_relevances.front().relevance : 0;

	// вычислить относительную релевантность для всех документов
	const auto size = abs_relevances.size();
	const auto itr_begin = abs_relevances.cbegin();
	const auto itr_end = size > max_responses ? (itr_begin + max_responses) : abs_relevances.cend();
	auto rel_relevancies
		= accumulate(itr_begin,
					 itr_end,
					 vector<RelativeRelevance>(),
					 [max_abs_relevance](auto& accum, const auto& abs_relevance) {
					   accum.emplace_back(
						   RelativeRelevance{abs_relevance.doc_id,
											 max_abs_relevance != 0
												 ? (float) (((float) abs_relevance.relevance
															 / (float) max_abs_relevance))
												 : (float) 0.0});
					   return accum;
					 });


    search_result.push_back(rel_relevancies);
  }

  return search_result;
}
}
