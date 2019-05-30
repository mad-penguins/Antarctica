cmake_minimum_required(VERSION 3.13)
project(antarctica)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp ui/MainWindow.cpp ui/MainWindow.h ui/LoginWindow.cpp ui/LoginWindow.h utils/LoginUtil.cpp utils/LoginUtil.h)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

add_subdirectory(api)

set(CMAKE_AUTOMOC ON)

include_directories(icebreaker)

add_executable(antarctica ${SOURCE_FILES})

target_link_libraries(antarctica icebreaker Qt5::Core Qt5::Widgets Qt5::Network)