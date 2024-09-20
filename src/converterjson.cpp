#include <fstream>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <set>

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

nlohmann::json ConverterJSON::readJsonFile(const std::string& fileName,
                                           const bool isRequired = false)
{
  nlohmann::json result;
  if (!std::filesystem::is_regular_file(fileName)) {
    std::cerr << "The file " << fileName << " is missing\n";

	if (isRequired) {
	  throw ConfigFileMissingException(fileName);
	}
  }

  std::ifstream file(fileName);

  if (file.is_open()) {
    result = nlohmann::json::parse(file);
  }

  return result;
}

std::filesystem::path ConverterJSON::getParentPath(const std::string& fileName) const
{
  return std::filesystem::path(fileName).parent_path();
}

void ConverterJSON::loadConfig(const nlohmann::json data)
{
  bool hasConfigSection = false;
  bool hasFilesSection  = false;

  if (data.size() == 0) { throw ConfigFileIsEmptyException(configFilename); }
  for (auto configItr = data.begin(); configItr != data.end(); configItr++) {
    const std::string propertyName = configItr.key();
    const auto propertyValue       = configItr.value();

	if (!hasConfigSection && propertyName == "config") hasConfigSection = true;
	if (!hasFilesSection && propertyName == "files") hasFilesSection = true;

	if (propertyName == "config") {
	  if (propertyValue.count("max_responses") == 0) throw NoMaxResponsesException(configFilename);
	  converterConfig.name         = propertyValue["name"];
	  converterConfig.version      = propertyValue["version"];
	  converterConfig.maxResponses = propertyValue["max_responses"];

	  if (converterConfig.version != VERSION) throw IncorrectVersionException(configFilename);
	} else if (propertyName == "files") {
	  converterConfig.files = propertyValue;
    }
  }
  if (!hasConfigSection) throw ConfigFileIsEmptyException(configFilename);
  if (!hasFilesSection) throw FilesSectionMissingException(configFilename);
}

std::string ConverterJSON::checkRequiredParameter(
  const std::string paramName,
  const std::string paramValue
)
{
	if (paramValue.empty()) {
	  std::stringstream ss;
	  ss << "The required config param " << paramName << " should have value" << std::endl;
	  throw std::invalid_argument(ss.str());
  }

  return paramValue;
}

int ConverterJSON::checkRequiredParameter(const std::string paramName, const int paramValue)
{
	if (paramValue <= 0) {
	  std::stringstream ss;
	  ss << "The required config param " << paramName << " should have value more then 0"
		 << std::endl;
	  throw std::invalid_argument(ss.str());
  }

  return paramValue;
}

std::vector<std::string> ConverterJSON::loadRequests(const nlohmann::json data)
{
  std::vector<std::string> requests;
  if (data.size() > 0) {
    for (auto requestsItr = data.begin(); requestsItr != data.end(); requestsItr++) {
      const std::string propertyName = requestsItr.key();
      const auto propertyValue = requestsItr.value();

	  if (propertyName == "requests") {
		std::vector<std::string> requestsRawLines = propertyValue;
		for (std::string requestLine : requestsRawLines) {
		  requests.push_back(requestLine);
		}
	  }
	}
  }
  return requests;
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
  return loadRequests(readJsonFile(requestsFilename));
}
