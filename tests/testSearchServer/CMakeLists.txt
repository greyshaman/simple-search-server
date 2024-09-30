cmake_minimum_required(VERSION 3.20)

project(TestSearchServer LANGUAGES CXX)

include("../../cmake/utils.cmake")

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
  enable_testing()
endif()

set(BUILD_SHARED_LIBS OFF)

# ## tests dependencies ## #

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(is_top_level)
  find_package(SearchEngine REQUIRED)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
configure_file(config-test.h.in config-test.h @ONLY)

set(TEST_CONVERTER_JSON_SRC
  test_search_server.h
  test_converterjson.cpp
)

list(APPEND TEST_CONVERTER_JSON_FILES
  ${TEST_CONVERTER_JSON_SRC}
  confs/wo_required_config.json
  confs/incorrect_version_config.json
  confs/good_config.json
  confs/wo_files_section_config.json
  confs/missed_data_file_config.json
  confs/no_max_responses_config.json
  requests/file001.txt
  requests/no_requests_section.json
  requests/long_requests_lines.json
  requests/requests.json
  requests/many_requests.json
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${TEST_CONVERTER_JSON_FILES})

list(APPEND TEST_CONVERTER_JSON_SRC
  "${TEST_CONVERTER_JSON_SRC}"
  "${CMAKE_CURRENT_BINARY_DIR}/config-test.h"
)

# test targets

#---------------
add_executable(TestConverterJson)
target_sources(TestConverterJson PRIVATE ${TEST_CONVERTER_JSON_SRC})
target_link_libraries(TestConverterJson
  PRIVATE
    search-engine::SearchEngine
    Qt${QT_VERSION_MAJOR}::Test
)
add_test(
  NAME TestConverterJson
  COMMAND TestConverterJson
)

#---------------
add_executable(TestInvertedIndex)
target_sources(TestInvertedIndex PRIVATE
  test_inverted_index.cpp
)
target_link_libraries(TestInvertedIndex
  PRIVATE
    search-engine::SearchEngine
    Qt${QT_VERSION_MAJOR}::Test
)
add_test(
  NAME TestInvertedIndex
  COMMAND TestInvertedIndex
)

#---------------
add_executable(TestSearchServer)
target_sources(TestSearchServer PRIVATE
  test_search_server.h test_search_server.cpp
)
target_link_libraries(TestSearchServer
  PRIVATE
  search-engine::SearchEngine
  Qt${QT_VERSION_MAJOR}::Test
)
add_test(
  NAME TestSearchServer
  COMMAND TestSearchServer
)
