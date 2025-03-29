cmake_minimum_required(VERSION 3.10)  # Минимальная версия CMake
project(RobotColorizer)  # Название проекта

# Указываем стандарт C++ (например, C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем исходные файлы
set(SOURCES
    colorizer.cpp
    main.cpp
    reciever.cpp
    server.cpp
)

# Добавляем заголовочные файлы
include_directories(vscode)

# Создаем исполняемый файл
add_executable(RobotColorizer ${SOURCES})
