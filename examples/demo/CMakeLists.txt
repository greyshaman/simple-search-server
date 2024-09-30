cmake_minimum_required(VERSION 3.20)

project(search-engine-demo VERSION 0.1 LANGUAGES CXX)

include(../../cmake/utils.cmake)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(is_top_level)
  find_package(SearchEngine REQUIRED)
endif()

set(sources main.cpp)

set(data_files
  config.json
  requests.json
  file001.txt
  file002.txt
  file003.txt
  file004.txt
)

list(APPEND project_files
  ${sources}
  ${data_files}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES "${project_files}")

add_executable(search-engine-demo)
target_sources(search-engine-demo PRIVATE ${sources})
target_link_libraries(search-engine-demo PRIVATE search-engine::SearchEngine)

# execute_process(COMMAND ${CMAKE_COMMAND}
#   -E copy_if_different ${data_files} "${CMAKE_CURRENT_BINARY_DIR}"
# )
file(COPY ${data_files} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
message("!!!!! check path: ${search-engine_SOURCE_DIR}")

if(NOT is_top_level)
  win_copy_deps_to_target_dir(search-engine-demo search-engine::SearchEngine)
endif()
