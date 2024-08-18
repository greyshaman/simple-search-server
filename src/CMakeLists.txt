include_directories(include)

set(INCLUDE_DIR include)

add_library(SearchServerLib SHARED
  ${INCLUDE_DIR}/converterjson.h converterjson.cpp
  ${INCLUDE_DIR}/config.h
  ${INCLUDE_DIR}/config-file-missing-exception.h config-file-missing-exception.cpp
)

add_executable(search-server
  main.cpp
)

target_link_libraries(
  search-server
  Qt${QT_VERSION_MAJOR}::Core
  SearchServerLib
  nlohmann_json::nlohmann_json
)
