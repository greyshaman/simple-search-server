#include "converter-config.h"

namespace search_server {
namespace converter_config {

void from_json(const json& j, ConverterConfig& config)
{
  if (j.contains("config")) {
    const auto configNode = j.at("config");
    configNode.at("name").get_to(config.name);
    configNode.at("version").get_to(config.version);
    if (configNode.contains("max_responses")) {
      configNode.at("max_responses").get_to(config.maxResponses);
    } else {
      config.maxResponses = -1;
    }
  }

  if (j.contains("files")) {
    j.at("files").get_to(config.files);
  } else {
    config.files = std::vector<ResourceFilename>();
  }
}

ConverterConfig::ConverterConfig()
	: name(""),
	version(""),
	maxResponses(-1),
	files(std::vector<ResourceFilename>()) {}

}
}

