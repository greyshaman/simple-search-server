#pragma once

#include <string>
#include <vector>

namespace search_server {
typedef std::string ResourceFilename;

struct ConverterConfig {
  std::string name;
  std::string version;
  int maxResponses = 0;
  std::vector<ResourceFilename> files;
};
}
