#include <fstream>
#include <filesystem>
#include <iostream>

#include "converterjson.h"
#include "config-file-missing-exception.h"

void ConverterJSON::readConfig() {
	if (!std::filesystem::is_regular_file(configFilename)) {
	  std::cerr << "config file missing" << std::endl;
	  throw ConfigFileMissingException(configFilename);
  }
  std::ifstream file(configFilename);
  if (file.is_open()) {

  }
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
  readConfig();
}

ConverterJSON::~ConverterJSON() {}

std::vector<std::string> ConverterJSON::GetTextDocuments() {

}
