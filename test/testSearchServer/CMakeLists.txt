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
  wo_required_config.json
  incorrect_version_config.json
  good_config.json
  wo_files_section_config.json
)
target_link_libraries(TestSearchServer PRIVATE SearchServerLib)
add_test(
  NAME TestSearchServer
  COMMAND TestSearchServer
)

target_link_libraries(TestSearchServer PRIVATE Qt${QT_VERSION_MAJOR}::Test)

