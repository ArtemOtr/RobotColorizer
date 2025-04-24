cmake_minimum_required(VERSION 3.8)

# Политика для MSVC
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(RobotColorizer VERSION 1.0 LANGUAGES CXX)

# Стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Кодировка исходников
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-finput-charset=UTF-8>")

# Указание исходников
set(SOURCES
    main.cpp
    colorizer.cpp
    robot_controller.cpp
    server.cpp
    mqtt_handler.cpp
)

# Создаём исполняемый файл
add_executable(RobotColorizer ${SOURCES})

# Подключаем библиотеки напрямую
target_link_libraries(RobotColorizer
    PRIVATE 
        mosquittopp
        mosquitto
)

# Windows defines
if(WIN32)
    target_compile_definitions(RobotColorizer PRIVATE "WIN32_LEAN_AND_MEAN")
endif()
