#include <fstream>
#include <filesystem>
#include <iostream>
#include <numeric>

#include "converterjson.h"
#include "config-file-missing-exception.h"
#include "config-file-is-empty-exception.h"
#include "incorrect-version-exception.h"
#include "no-max-responses-exception.h"
#include "files-section-missing-exception.h"
#include "config.h"

const std::string VERSION = std::string(
  #ifdef PROJECT_VERSION
    PROJECT_VERSION
  #else
    "<unknown version>"
  #endif
);

const size_t BUFF_SIZE = 1024;

const size_t MAX_REQUESTS = 1000;
const size_t MAX_WORDS_PER_REQUEST = 10;

using json = nlohmann::json;
using namespace search_server;

json ConverterJSON::readJsonFile(const std::string& filename,
                                           const bool is_required = false) const
{
  json result;
  if (!std::filesystem::is_regular_file(filename)) {
    std::cerr << "The file " << filename << " is missing\n";

	if (is_required) {
	  throw ConfigFileMissingException(filename);
	}
  }

  std::ifstream file(filename);

  if (file.is_open()) {
    result = json::parse(file);
  }

  return result;
}

std::filesystem::path ConverterJSON::getParentPath(const std::string& filename) const
{
  return std::filesystem::path(filename).parent_path();
}

void ConverterJSON::loadConfig(const json& json_config)
{
  if (json_config.size() == 0 || !json_config.contains("config")) {
    throw ConfigFileIsEmptyException(config_filename);
  }
  converter_config = json_config.template get<search_server::converter_config::ConverterConfig>();

  if (converter_config.version != VERSION) {
    throw IncorrectVersionException(config_filename);
  }

  if (converter_config.max_responses < 1) {
    throw NoMaxResponsesException(config_filename);
  }

  if (converter_config.files.empty()) {
    throw FilesSectionMissingException(config_filename);
  }
}

search_server::requests_format::RequestsConfig ConverterJSON::loadRequests(
    const nlohmann::json& json_requests)
{
  requests_format::RequestsConfig requests_config;
  if (json_requests.size() > 0 && json_requests.contains("requests")) {
    requests_config = json_requests.template get<requests_format::RequestsConfig>();
  }

  if (requests_config.requests.size() > MAX_REQUESTS) {
    auto& requests = requests_config.requests;
    requests.resize(MAX_REQUESTS);
    requests.shrink_to_fit();
  }

  for (auto& request : requests_config.requests) {
    truncToNWords(request, MAX_WORDS_PER_REQUEST);
  }

  return requests_config;
}

ConverterJSON::ConverterJSON() : ConverterJSON("config.json", "requests.json", "answers.json") {}

ConverterJSON::ConverterJSON(
  std::string in_config_filename,
  std::string in_requests_filename,
  std::string in_answers_filename
)
  : config_filename(in_config_filename)
  , requests_filename(in_requests_filename)
  , answers_filename(in_answers_filename)
{
  loadConfig(readJsonFile(config_filename, true));
}

ConverterJSON::~ConverterJSON() {}

std::vector<std::string> ConverterJSON::GetTextDocuments()
{
  std::vector<std::string> docs;
  if (converter_config.files.size() > 0) {
      for (auto& filename : converter_config.files) {
        std::filesystem::path filepath(filename);

		if (filepath.is_relative()) {
		  const auto config_dir = getParentPath(config_filename);
		  std::filesystem::path absolute_resource_path(config_dir);
		  absolute_resource_path.append(filename);
		  filepath = absolute_resource_path;
		}

		if (std::filesystem::is_regular_file(filepath)) {
		  std::ifstream resource_file(filepath);
		  if (resource_file.is_open()) {
			std::stringstream doc_strem;
			char buff[BUFF_SIZE];
			while (!resource_file.eof()) {
			  resource_file.getline(buff, BUFF_SIZE);
			  doc_strem << buff;
			}
			docs.emplace_back(doc_strem.str());
		  } else {
			std::cerr << "Cannot open file " << filename << std::endl;
		  }
		} else {
		  std::cerr << "File " << filename << " not found\n";
		}
	}
  }
  return docs;
}

int ConverterJSON::GetResponsesLimit()
{
  return converter_config.max_responses;
}

std::vector<std::string> ConverterJSON::GetRequests()
{
  requests_config = loadRequests(readJsonFile(requests_filename));
  return requests_config.requests;
}

void ConverterJSON::PutAnswers(std::vector<std::vector<std::pair<int, float>>> answers)
{
  json json_answers = prepareAnswersToExport(answers);

  std::ofstream file(answers_filename, std::ios::out | std::ios::trunc);

  if (file.is_open()) {
    // TODO remove setw(4)
    file << std::setw(4) << json_answers;
  }
}

json ConverterJSON::prepareAnswersToExport(std::vector<std::vector<std::pair<int, float>>> answers)
{
  json json_answers;

  int requests_counter = 0;
  for (std::vector<std::pair<int, float>> answer : answers) {
    std::stringstream request_name_strm;
    request_name_strm << "request" << std::setw(3) << std::setfill('0') << ++requests_counter;

	if (answer.empty()) {
	  const json json_result_node = json::object({{"result", "false"}});
	  const json json_requst_node = json::object({{request_name_strm.str(), json_result_node}});
	  json_answers.merge_patch(json_requst_node);
	} else if (answer.size() == 1) {
	  const auto answer_itr = answer.begin();
	  const int doc_id = (*answer_itr).first;
	  const float rank = (*answer_itr).second;
	  const json json_result_node = json::object({{"result", "true"}, {"docid", doc_id}, {"rank", rank}});
	  const json json_requst_node = json::object({{request_name_strm.str(), json_result_node}});
	  json_answers.merge_patch(json_requst_node);
	} else {
	  json json_result_node = json::object({{"result", "true"}});
	  std::vector<json> relevance_list;
	  for (auto result_pair : answer) {
		const auto doc_id = result_pair.first;
		const auto rank = result_pair.second;
		relevance_list.push_back(json::object({{"docid", doc_id}, {"rank", rank}}));
	  }
	  json_result_node.merge_patch(json::object({{"relevance", relevance_list}}));
	  const json json_requst_node = json::object({{request_name_strm.str(), json_result_node}});
	  json_answers.merge_patch(json_requst_node);
	}
  }

  return json::object({{"answers", json_answers}});
}

void ConverterJSON::truncToNWords(std::string& string_to_trunk, const size_t n)
{
  std::istringstream iss(string_to_trunk);
  std::vector<std::string> words;
  std::string word;

  while (iss >> word) {
    words.push_back(word);
  }

  if (words.size() > n) {
    string_to_trunk = std::accumulate(words.cbegin(),
                                   words.cbegin() + n,
                                   std::string(),
                                   [](std::string acc, const std::string& word) {
                                     return acc + " " + word;
                                   });
  }
}
