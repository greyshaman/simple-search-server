cmake_minimum_required(VERSION 3.20)

project(TestSearchServer LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
configure_file(config-test.h.in config-test.h @ONLY)

include_directories(${INCLUDE_DIR})

add_executable(TestConverterJson
  test_search_server.h
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
  requests/many_requests.json
)

add_executable(TestInvertedIndex
  test_inverted_index.cpp
)

add_executable(TestSearchServer
  test_search_server.h test_search_server.cpp
)

target_link_libraries(TestConverterJson PRIVATE SearchServerLib)
add_test(
  NAME TestConverterJson
  COMMAND TestConverterJson
)

target_link_libraries(TestSearchServer PRIVATE SearchServerLib)
add_test(
  NAME TestSearchServer
  COMMAND TestSearchServer
)

target_link_libraries(TestInvertedIndex PRIVATE SearchServerLib)
add_test(
  NAME TestInvertedIndex
  COMMAND TestInvertedIndex
)

target_link_libraries(TestConverterJson PRIVATE Qt${QT_VERSION_MAJOR}::Test)

target_link_libraries(TestSearchServer PRIVATE Qt${QT_VERSION_MAJOR}::Test)

target_link_libraries(TestInvertedIndex PRIVATE Qt${QT_VERSION_MAJOR}::Test)

