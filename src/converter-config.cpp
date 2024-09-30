#include "converter-config.h"

namespace search_server {
namespace converter_config {

void from_json(const json& json_source, ConverterConfig& config)
{
  if (json_source.contains("config")) {
    const auto config_node = json_source.at("config");
    config_node.at("name").get_to(config.name);
    config_node.at("version").get_to(config.version);
    if (config_node.contains("max_responses")) {
      config_node.at("max_responses").get_to(config.max_responses);
    } else {
      config.max_responses = 0;
    }
  }

  if (json_source.contains("files")) {
    json_source.at("files").get_to(config.files);
  } else {
    config.files = std::vector<ResourceFilename>();
  }
}

ConverterConfig::ConverterConfig()
	: name(""),
	version(""),
	max_responses(0),
	files(std::vector<ResourceFilename>()) {}

}
}

