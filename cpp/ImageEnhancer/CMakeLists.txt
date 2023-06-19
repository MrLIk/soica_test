cmake_minimum_required(VERSION 3.15)

project(SoicaTest LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/backend)

add_executable(console_enhancer
    src/main_console.cpp

    src/ImageEnhancer.h
    src/ImageEnhancer.cpp
)

target_link_libraries(console_enhancer PUBLIC
    backend
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
)

add_executable(gui_enhancer
    src/main_gui.cpp

    src/ImageEnhancer.h
    src/ImageEnhancer.cpp

    src/MainWindow.h
    src/MainWindow.cpp
)

target_link_libraries(gui_enhancer PUBLIC
    backend
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)
