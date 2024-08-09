#pragma once

#include <string>
#include <vector>

struct Config {
  std::string name;
  std::string version;
  int maxResponses = 5;
  std::vector<std::string> files;
};
