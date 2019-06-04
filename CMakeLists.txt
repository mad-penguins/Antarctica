cmake_minimum_required(VERSION 3.13)
project(antarctica)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp ui/MainWindow.cpp ui/MainWindow.h ui/LoginWindow.cpp ui/LoginWindow.h utils/SSL.hpp ui/models/FileTreeModel.cpp ui/models/FileTreeModel.h ui/models/PackageTreeItem.cpp ui/models/PackageTreeItem.h ui/models/PackageTreeModel.cpp ui/models/PackageTreeModel.h ui/models/TreeItem.cpp ui/models/TreeItem.h ui/models/TreeModel.cpp ui/models/TreeModel.h ui/models/FileTreeItem.cpp ui/models/FileTreeItem.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

add_subdirectory(api)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(antarctica ${SOURCE_FILES} resources/resources.qrc)

target_link_libraries(antarctica icebreaker Qt5::Core Qt5::Widgets Qt5::Network)
