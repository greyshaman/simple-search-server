#include <fstream>
#include <filesystem>
#include <iostream>
#include <sstream>

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

void ConverterJSON::readConfigFile() {
	if (!std::filesystem::is_regular_file(configFilename)) {
	  std::cerr << "config file missing" << std::endl;
	  throw ConfigFileMissingException(configFilename);
  }
  std::ifstream file(configFilename);
  if (file.is_open()) {
    loadConfig(nlohmann::json::parse(file));
  } else {
    throw std::runtime_error("Cannot open config file");
  }
}

void ConverterJSON::loadConfig(const nlohmann::json data) {
  bool hasConfigSection = false;
  bool hasFilesSection  = false;

  if (data.size() == 0) { throw ConfigFileIsEmptyException(configFilename); }
  for (auto configItr = data.begin(); configItr != data.end(); configItr++) {
    const std::string propertyName = configItr.key();
    const auto propertyValue       = configItr.value();
    hasConfigSection               = (!hasConfigSection && propertyName == "config");
    hasFilesSection                = (!hasFilesSection && propertyName == "files");

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
) {
	if (paramValue.empty()) {
	  std::stringstream ss;
	  ss << "The required config param " << paramName << " should have value" << std::endl;
	  throw std::invalid_argument(ss.str());
  }

  return paramValue;
}

int ConverterJSON::checkRequiredParameter(const std::string paramName, const int paramValue) {
	if (paramValue <= 0) {
	  std::stringstream ss;
	  ss << "The required config param " << paramName << " should have value more then 0"
		 << std::endl;
	  throw std::invalid_argument(ss.str());
  }

  return paramValue;
}

ConverterJSON::ConverterJSON() : ConverterJSON("config.json", "requests.json", "answers.json") {}

ConverterJSON::ConverterJSON(
  std::string inConfigFilename,
  std::string inRequestsFilename,
  std::string inAnswersFilename
)
  : configFilename(inConfigFilename)
  , requestsFilename(inRequestsFilename)
  , answersFilename(inAnswersFilename) {
  readConfigFile();
}

ConverterJSON::~ConverterJSON() {}

// std::vector<std::string> ConverterJSON::GetTextDocuments() {

// }
