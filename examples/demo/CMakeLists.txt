cmake_minimum_required(VERSION 3.20)

project(demo VERSION 0.1 LANGUAGES CXX)

# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

set(JSON_PACKAGE_DIR "../../nlohmann_json")
add_subdirectory(${JSON_PACKAGE_DIR})
include_directories(SYSTEM ${JSON_PACKAGE_DIR}/include)

include_directories(../../include)
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory(src)
add_subdirectory(tests)

set(DEMO_FILES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/demo)
list(DEMO_FILES APPEND
  ${DEMO_FILES_DIR}/config.json
  ${DEMO_FILES_DIR}/requests.json
  ${DEMO_FILES_DIR}/file001.txt
  ${DEMO_FILES_DIR}/file002.txt
  ${DEMO_FILES_DIR}/file003.txt
  ${DEMO_FILES_DIR}/file004.txt
)

include(GNUInstallDirs)
install(TARGETS search-server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

