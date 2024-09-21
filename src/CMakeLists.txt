set(INCLUDE_DIR include)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(EXCEPTIONS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/exceptions)
set(EXCEPTIONS_INCLUDE_DIR ${EXCEPTIONS_DIR}/include)

include_directories(include ${EXCEPTIONS_INCLUDE_DIR})

configure_file(config.h.in config.h @ONLY)

add_library(SearchServerLib SHARED
  config.h.in
  ${INCLUDE_DIR}/converterjson.h converterjson.cpp
  ${INCLUDE_DIR}/converter-config.h converter-config.cpp
  ${INCLUDE_DIR}/requests.h
  ${INCLUDE_DIR}/inverted-index.h inverted-index.cpp
  ${EXCEPTIONS_INCLUDE_DIR}/config-parser-exception.h ${EXCEPTIONS_DIR}/config-parser-exception.cpp
  ${EXCEPTIONS_INCLUDE_DIR}/config-file-missing-exception.h ${EXCEPTIONS_DIR}/config-file-missing-exception.cpp
  ${EXCEPTIONS_INCLUDE_DIR}/config-file-is-empty-exception.h ${EXCEPTIONS_DIR}/config-file-is-empty-exception.cpp
  ${EXCEPTIONS_INCLUDE_DIR}/incorrect-version-exception.h ${EXCEPTIONS_DIR}/incorrect-version-exception.cpp
  ${EXCEPTIONS_INCLUDE_DIR}/no-max-responses-exception.h ${EXCEPTIONS_DIR}/no-max-responses-exception.cpp
  ${EXCEPTIONS_INCLUDE_DIR}/files-section-missing-exception.h ${EXCEPTIONS_DIR}/files-section-missing-exception.cpp
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
