cmake_minimum_required(VERSION 3.10)

project(Robot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(PROJECT_NAME Robot)

add_executable(${PROJECT_NAME}
    Robot.cpp
)

include(FetchContent)
set(FETCHCONTENT_QUIET ON)

#FetchContent_Declare(
#    mosquitto
#    GIT_REPOSITORY https://github.com/eclipse/mosquitto.git
#    GIT_TAG a196c2b244f248072a6b3ac8fb3f00ce0ff63dea
#)
set(WITH_DOCS OFF CACHE BOOL "[Mosquitto] Disablee documentation build" FORCE)
set(DOCUMENTATION OFF CACHE BOOL "[Mosquitto] Docs" FORCE)
set(WITH_APPS OFF CACHE BOOL "[Mosquitto] App build" FORCE)
set(WITH_CJSON OFF CACHE BOOL "[Mosquitto] Disablee JSON build" FORCE)
set(WITH_STATIC_LIBRARIES ON CACHE BOOL "")
set(WITH_PIC  ON CACHE BOOL "")

set(WITH_SHARED_LIBRARIES ON CACHE BOOL "[Mosquitto] lib" FORCE)

set(WITH_TLS OFF CACHE BOOL "[Mosquitto] SSL/TLS support" FORCE)
	

set(CMAKE_EXE_LINKER_FLAGS "-static")

#FetchContent_MakeAvailable(mosquitto)

#FetchContent_GetProperties(mosquitto
#    POPULATED mosquitto_POPULATED
#    SOURCE_DIR mosquitto_SOURCE_DIR
#    BINARY_DIR mosquitto_BINARY_DIR
#)
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/mosquitto/include
        ${mosquitto_SOURCE_DIR}/include

        ${mosquitto_SOURCE_DIR}/include
        ${mosquitto_SOURCE_DIR}/lib
        ${mosquitto_SOURCE_DIR}/lib/cpp
        ${mosquitto_BINARY_DIR}/lib
        ${mosquitto_BINARY_DIR}/lib/cpp
        ${mosquitto_BINARY_DIR}
)

message("DIR================== ${mosquittopp_BINARY_DIR}")


target_link_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/mosquitto/build/lib
        ${CMAKE_SOURCE_DIR}/mosquitto/build/lib/cpp
        ${mosquitto_SOURCE_DIR}/include
        ${mosquitto_SOURCE_DIR}/lib
        ${mosquitto_SOURCE_DIR}/lib/cpp
        ${mosquitto_BINARY_DIR}
)


#target_link_directoriess(${PROJECT_NAME} PRIVATE mosquittopp_static.a)

target_link_libraries(${PROJECT_NAME} PRIVATE mosquittopp_static)
target_link_libraries(${PROJECT_NAME} PRIVATE mosquitto_static)

target_link_libraries(${PROJECT_NAME} PRIVATE pthread)


target_include_directories(Robot PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")