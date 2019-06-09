cmake_minimum_required(VERSION 3.13)
project(antarctica)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp ui/MainWindow.cpp ui/MainWindow.h ui/LoginWindow.cpp ui/LoginWindow.h utils/SSL.hpp ui/models/files/FileTreeModel.cpp ui/models/files/FileTreeModel.h ui/models/packages/PackageTreeItem.cpp ui/models/packages/PackageTreeItem.h ui/models/packages/PackageTreeModel.cpp ui/models/packages/PackageTreeModel.h ui/models/TreeItem.cpp ui/models/TreeItem.h ui/models/TreeModel.cpp ui/models/TreeModel.h ui/models/files/FileTreeItem.cpp ui/models/files/FileTreeItem.h utils/Files.hpp ui/AddPackageDialog.cpp ui/AddPackageDialog.h ui/PackageConfigurator.cpp ui/PackageConfigurator.h utils/Repositories.hpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

add_subdirectory(api)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(antarctica ${SOURCE_FILES} resources/resources.qrc)

target_link_libraries(antarctica icebreaker Qt5::Core Qt5::Widgets Qt5::Network)
