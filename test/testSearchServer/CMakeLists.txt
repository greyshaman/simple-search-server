cmake_minimum_required(VERSION 3.20)

project(TestSearchServer LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

configure_file(config-test.h.in config-test.h @ONLY)

include_directories(../../src/include ../../src/exceptions/include)

add_executable(TestSearchServer
  test_converterjson.cpp
  config-test.h.in
  confs/wo_required_config.json
  confs/incorrect_version_config.json
  confs/good_config.json
  confs/wo_files_section_config.json
  confs/missed_data_file_config.json
  confs/no_max_responses_config.json
  resources/file001.txt
  requests/no_requests_section.json
  requests/long_requests_lines.json
  requests/requests.json
)
target_link_libraries(TestSearchServer PRIVATE SearchServerLib)
add_test(
  NAME TestSearchServer
  COMMAND TestSearchServer
)

target_link_libraries(TestSearchServer PRIVATE Qt${QT_VERSION_MAJOR}::Test)

