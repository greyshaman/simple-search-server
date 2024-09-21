#include <fstream>
#include <filesystem>
#include <iostream>

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

const unsigned int MAX_REQUESTS_PER_LINE = 10;

using json = nlohmann::json;
using namespace search_server;

json ConverterJSON::readJsonFile(const std::string& fileName,
                                           const bool isRequired = false)
{
  json result;
  if (!std::filesystem::is_regular_file(fileName)) {
    std::cerr << "The file " << fileName << " is missing\n";

	if (isRequired) {
	  throw ConfigFileMissingException(fileName);
	}
  }

  std::ifstream file(fileName);

  if (file.is_open()) {
    result = json::parse(file);
  }

  return result;
}

std::filesystem::path ConverterJSON::getParentPath(const std::string& fileName) const
{
  return std::filesystem::path(fileName).parent_path();
}

void ConverterJSON::loadConfig(const json jdata)
{
  if (jdata.size() == 0 || !jdata.contains("config")) {
    throw ConfigFileIsEmptyException(configFilename);
  }
  converterConfig = jdata.template get<search_server::converter_config::ConverterConfig>();

  if (converterConfig.version != VERSION) {
    throw IncorrectVersionException(configFilename);
  }

  if (converterConfig.maxResponses < 1) {
    throw NoMaxResponsesException(configFilename);
  }

  if (converterConfig.files.empty()) {
    throw FilesSectionMissingException(configFilename);
  }
}

search_server::requests_format::RequestsConfig ConverterJSON::loadRequests(const nlohmann::json jdata)
{
  using namespace search_server::requests_format;
  RequestsConfig requestsConfig;
  if (jdata.size() > 0 && jdata.contains("requests")) {
    requestsConfig = jdata.template get<RequestsConfig>();
  }
  return requestsConfig;
}

ConverterJSON::ConverterJSON() : ConverterJSON("config.json", "requests.json", "answers.json") {}

ConverterJSON::ConverterJSON(
  std::string inConfigFilename,
  std::string inRequestsFilename,
  std::string inAnswersFilename
)
  : configFilename(inConfigFilename)
  , requestsFilename(inRequestsFilename)
  , answersFilename(inAnswersFilename)
{
  loadConfig(readJsonFile(configFilename, true));
}

ConverterJSON::~ConverterJSON() {}

std::vector<std::string> ConverterJSON::GetTextDocuments()
{
  std::vector<std::string> actualFileList;
  if (converterConfig.files.size() > 0) {
      for (auto filename : converterConfig.files) {
        std::filesystem::path filepath(filename);

		if (filepath.is_relative()) {
		  const auto configDir = getParentPath(configFilename);
		  std::filesystem::path absoluteResourcePath(configDir);
		  absoluteResourcePath.append(filename);
		  filepath = absoluteResourcePath;
		}

		if (std::filesystem::is_regular_file(filepath)) {
		  actualFileList.push_back(filepath);
		} else {
		  std::cerr << "File " << filename << " not found\n";
		}
	}
  }
  return actualFileList;
}

std::vector<std::string> ConverterJSON::GetRequests()
{
  requestsConfig = loadRequests(readJsonFile(requestsFilename));
  return requestsConfig.requests;
}

void ConverterJSON::PutAnswers(std::vector<std::vector<std::pair<int, float>>> answers)
{
  json jAnswers = prepareAnswersToExport(answers);

  std::ofstream file(answersFilename, std::ios::out | std::ios::trunc);

  if (file.is_open()) {
    file << std::setw(4) << jAnswers;
  }
}

json ConverterJSON::prepareAnswersToExport(std::vector<std::vector<std::pair<int, float>>> answers)
{
  json j;

  int requestsCounter = 0;
  for (std::vector<std::pair<int, float>> answer : answers) {
    std::stringstream requestNameStrm;
    requestNameStrm << "request" << std::setw(3) << std::setfill('0') << ++requestsCounter;

	if (answer.empty()) {
	  const json resultRec = json::object({{"result", "false"}});
	  const json jRequstRow = json::object({{requestNameStrm.str(), resultRec}});
	  j.merge_patch(jRequstRow);
	} else if (answer.size() == 1) {
	  const auto answerItr = answer.begin();
	  const int docId = (*answerItr).first;
	  const float rank = (*answerItr).second;
	  const json resultRec = json::object({{"result", "true"}, {"docid", docId}, {"rank", rank}});
	  const json jRequstRow = json::object({{requestNameStrm.str(), resultRec}});
	  j.merge_patch(jRequstRow);
	} else {
	  json resultRec = json::object({{"result", "true"}});
	  std::vector<json> relevanceList;
	  for (auto resultPair : answer) {
		const auto docId = resultPair.first;
		const auto rank = resultPair.second;
		relevanceList.push_back(json::object({{"docid", docId}, {"rank", rank}}));
	  }
	  resultRec.merge_patch(json::object({{"relevance", relevanceList}}));
	  const json jRequstRow = json::object({{requestNameStrm.str(), resultRec}});
	  j.merge_patch(jRequstRow);
	}
  }

  return json::object({{"answers", j}});
}
