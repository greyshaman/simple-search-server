#pragma once

#include <string>
#include <vector>

struct ConverterConfig {
  std::string name;
  std::string version;
  int maxResponses = 0;
  std::vector<std::string> files;
};
