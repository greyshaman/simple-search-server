cmake_minimum_required(VERSION 3.20)

project(search-engine
  VERSION 0.1
  DESCRIPTION "The Search Server engine is library of service with inverted index functionality"
  LANGUAGES CXX)

# ## general settings and options ## #

include(cmake/utils.cmake)
include(GNUInstallDirs)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

option(SEARCH_ENGINE_BUILD_TESTS "Build search-engine tests" OFF)
option(SEARCH_ENGINE_BUILD_EXAMPLES "Build search-engine library examples" OFF)
option(SEARCH_ENGINE_BUILD_DOCS "Build search-engine library documentation" OFF)
option(SEARCH_ENGINE_INSTALL
  "Generate target for installing search-engine library" ${is_top_level}
)

set_if_undefined(SEARCH_ENGINE_INSTALL_CMAKEDIR
  "${CMAKE_INSTALL_LIBDIR}/cmake/search-engine-${PROJECT_VERSION}" CACHE STRING
  "Install path for search-engine package-related CMake files"
)

if(DEFINED SEARCH_ENGINE_SHARED_LIBS)
  set(BUILD_SHARED_LIBS ${SEARCH_ENGINE_SHARED_LIBS})
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
  set_property(CACHE
    CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo"
  )
endif()

set_if_undefined(CMAKE_CXX_VISIBILITY_PRESET hidden)
set_if_undefined(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

add_library(SearchEngine) # init lib target below
add_library(search-engine::SearchEngine ALIAS SearchEngine)

# ## dependencies ## #

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(nlohmann_json)
include_directories(SYSTEM nlohmann_json/include)

# ## search-engine sources ## #

include(GenerateExportHeader)
set(export_file_name "export-shared.h")

if(NOT BUILD_SHARED_LIBS)
  set(export_file_name "export-static.h")
endif()

include_directories(include)
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(EXCEPTIONS_INCLUDE_DIR "${INCLUDE_DIR}/exceptions")

generate_export_header(SearchEngine
  EXPORT_FILE_NAME
  "${CMAKE_CURRENT_BINARY_DIR}/include/${export_file_name}")

configure_file("${INCLUDE_DIR}/config-version.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/include/config-version.h"
  @ONLY
)

set(public_headers
  "${INCLUDE_DIR}/export.h"
  "${INCLUDE_DIR}/converter-config.h"
  "${INCLUDE_DIR}/converterjson.h"
  "${INCLUDE_DIR}/inverted-index.h"
  "${INCLUDE_DIR}/requests.h"
  "${INCLUDE_DIR}/search-server.h"
  "${CMAKE_CURRENT_BINARY_DIR}/include/config-version.h"
  "${EXCEPTIONS_INCLUDE_DIR}/config-file-is-empty-exception.h"
  "${EXCEPTIONS_INCLUDE_DIR}/config-file-missing-exception.h"
  "${EXCEPTIONS_INCLUDE_DIR}/config-parser-exception.h"
  "${EXCEPTIONS_INCLUDE_DIR}/files-section-missing-exception.h"
  "${EXCEPTIONS_INCLUDE_DIR}/incorrect-version-exception.h"
  "${EXCEPTIONS_INCLUDE_DIR}/no-max-responses-exception.h"
)

set(sources
  ${public_headers}
  src/converter-config.cpp
  src/converterjson.cpp
  src/inverted-index.cpp
  src/search-server.cpp
  src/exceptions/config-file-is-empty-exception.cpp
  src/exceptions/config-file-missing-exception.cpp
  src/exceptions/config-parser-exception.cpp
  src/exceptions/files-section-missing-exception.cpp
  src/exceptions/incorrect-version-exception.cpp
  src/exceptions/no-max-responses-exception.cpp
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

list(APPEND public_headers
  "${CMAKE_CURRENT_BINARY_DIR}/include/${export_file_name}"
)
list(APPEND sources
  "${CMAKE_CURRENT_BINARY_DIR}/include/${export_file_name}"
)

# ## search-engine target ## #

include(CMakePackageConfigHelpers)

target_sources(SearchEngine PRIVATE ${sources})
target_compile_definitions(SearchEngine
  PUBLIC
    "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:SEARCH_ENGINE_STATIC_DEFINE>"
)

target_include_directories(SearchEngine
  PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

set_target_properties(SearchEngine PROPERTIES
  PUBLIC_HEADER "${public_headers}"
  SOVERSION ${PROJECT_VERSION_MAJOR}
  VERSION ${PROJECT_VERSION}
)

# ## install target ## #

if(SEARCH_ENGINE_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
  configure_package_config_file(cmake/search-engine-config.cmake.in search-engine-config.cmake
    INSTALL_DESTINATION "${SEARCH_ENGINE_INSTALL_CMAKEDIR}"
  )

  write_basic_package_version_file(search-engine-config-version.cmake
    COMPATIBILITY SameMajorVersion
  )

  install(TARGETS SearchEngine EXPORT SearchEngine_export
    RUNTIME COMPONENT SearchEngine
    LIBRARY COMPONENT SearchEngine NAMELINK_COMPONENT SearchEngine-dev
    ARCHIVE COMPONENT SearchEngine-dev
    PUBLIC_HEADER COMPONENT SearchEngine-dev
      DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/search-engine"
  )

  set(targets_file "search-engine-shared-targets.cmake")

  if(NOT BUILD_SHARED_LIBS)
    set(targets_file "search-engine-static-targets.cmake")
  endif()

  install(EXPORT SearchEngine_export
    COMPONENT SearchEngine-dev
    FILE "${targets_file}"
    DESTINATION "${SEARCH_ENGINE_INSTALL_CMAKEDIR}"
    NAMESPACE search-engine::
  )

  install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/search-engine-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/search-engine-config-version.cmake"
    COMPONENT SearchEngine-dev
    DESTINATION "${SEARCH_ENGINE_INSTALL_CMAKEDIR}"
  )

  if(MSVC)
    set(pdb_file "")
    set(pdb_file_destination "")

    if(BUILD_SHARED_LIBS)
      set(pdb_file "$<TARGET_PDB_FILE:search-engine>")
      set(pdb_file_destination "${CMAKE_INSTALL_BINDIR}")
    else()
      # TARGET_PDB_FILE does not work for pdb file generated for static library build, determining it manually
      set(pdb_file
        "$<TARGET_FILE_DIR:search-engine>/$<TARGET_FILE_PREFIX:search-engine>$<TARGET_FILE_BASE_NAME:search-engine>.pdb"
      )
      set(pdb_file_destination "${CMAKE_INSTALL_LIBDIR}")
    endif()

    install(FILES "${pdb_file}"
      COMPONENT SearchEngine-dev
      CONFIGURATIONS Debug RelWithDebInfo
      DESTINATION "${pdb_file_destination}"
      OPTIONAL)
  endif()
endif()


if(SEARCH_ENGINE_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if(SEARCH_ENGINE_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
