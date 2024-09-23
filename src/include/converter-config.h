#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace search_server {
namespace converter_config {

typedef std::string ResourceFilename;

struct ConverterConfig
{
  std::string name;
  std::string version;
  int max_responses = 0;
  std::vector<ResourceFilename> files;

  ConverterConfig();
};

void from_json(const json&, ConverterConfig&);

}
}
