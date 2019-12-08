cmake_minimum_required(VERSION 3.14)
project(elementary_cellular_automata)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_PREFIX_PATH "/home/matlle/Qt/5.12.5/gcc_64/lib/cmake")

set(SOURCE_FILES main.cpp headers/mainwindow.h sources/mainwindow.cpp)
add_executable(elementary_cellular_automata ${SOURCE_FILES})

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

target_link_libraries(elementary_cellular_automata Qt5::Core)
target_link_libraries(elementary_cellular_automata Qt5::Widgets)