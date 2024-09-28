#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace search_server {
namespace requests_format {

typedef std::string RequestsLine;

/**
 * @brief RequestsConfig структура используется для десериализации requests.json
 */
struct RequestsConfig
{
  std::vector<RequestsLine> requests;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RequestsConfig, requests)

}
}
