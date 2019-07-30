cmake_minimum_required(VERSION 3.13)
project(antarctica)

set(CMAKE_CXX_STANDARD 17)

set(UTILS_SOURCES
        utils/Files.hpp utils/Packages.hpp utils/Repositories.hpp utils/UI.hpp utils/SSL.hpp
        utils/FilesMonitor.cpp utils/FilesMonitor.h)

set(UI_SOURCES
        ui/MainWindow.cpp ui/MainWindow.h
        ui/LoginWindow.cpp ui/LoginWindow.h
        ui/settings/SettingsWindow.cpp ui/settings/SettingsWindow.h
        ui/models/files/FileTreeModel.cpp ui/models/files/FileTreeModel.h
        ui/models/packages/PackageTreeItem.cpp ui/models/packages/PackageTreeItem.h
        ui/models/packages/PackageTreeModel.cpp ui/models/packages/PackageTreeModel.h
        ui/models/TreeItem.cpp ui/models/TreeItem.h
        ui/models/TreeModel.cpp ui/models/TreeModel.h
        ui/models/files/FileTreeItem.cpp ui/models/files/FileTreeItem.h
        ui/packages/AddPackageDialog.cpp ui/packages/AddPackageDialog.h
        ui/packages/PackageConfigurator.cpp ui/packages/PackageConfigurator.h
        ui/settings/ProxyConfigurator.cpp ui/settings/ProxyConfigurator.h
        ui/settings/SectionConfigurator.h)

set(SOURCE_FILES main.cpp ${UTILS_SOURCES} ${UI_SOURCES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

add_subdirectory(api)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(antarctica ${SOURCE_FILES} resources/resources.qrc)
target_link_libraries(antarctica icebreaker Qt5::Core Qt5::Widgets Qt5::Network)

include(GenerateRPM.cmake)
