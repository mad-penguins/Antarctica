cmake_minimum_required(VERSION 3.13)
project(antarctica)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp MainWindow.cpp MainWindow.h LoginWindow.cpp LoginWindow.h utils/LoginUtil.cpp utils/LoginUtil.h models/User.h models/File.h models/Package.h models/Repository.h utils/api/APIWrapper.h utils/api/FilesWrapper.cpp utils/api/PackagesWrapper.cpp utils/api/RepositoriesWrapper.cpp utils/api/APIWrapper.cpp)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

set(CMAKE_AUTOMOC ON)

add_executable(antarctica ${SOURCE_FILES})

target_link_libraries(antarctica Qt5::Core Qt5::Widgets Qt5::Network)