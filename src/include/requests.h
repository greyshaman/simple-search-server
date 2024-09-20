#pragma once

#include <string>
#include <vector>

namespace search_server {
typedef std::string RequestsLine;

struct Requests
{
  std::vector<RequestsLine> requests;
};
}
