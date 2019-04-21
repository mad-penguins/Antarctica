cmake_minimum_required(VERSION 3.13)
project(antarctica)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp MainWindow.cpp MainWindow.h LoginWindow.cpp LoginWindow.h utils/LoginUtil.cpp utils/LoginUtil.h models/User.h utils/DatabaseUtil.cpp utils/DatabaseUtil.h DAO/FileDAO.cpp DAO/FileDAO.h DAO/DAO.h models/File.h)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Sql REQUIRED)

set(CMAKE_AUTOMOC ON)

add_executable(antarctica ${SOURCE_FILES})

target_link_libraries(antarctica Qt5::Core Qt5::Widgets Qt5::Network Qt5::Sql)