cmake_minimum_required(VERSION 3.16)
project(JuegoEcuaciones LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Charts)

file(GLOB FUENTES CONFIGURE_DEPENDS src/*.cpp include/*.h)

add_executable(JuegoEcuaciones ${FUENTES})
target_include_directories(JuegoEcuaciones PRIVATE include)
target_link_libraries(JuegoEcuaciones PRIVATE Qt6::Widgets Qt6::Sql Qt6::Charts)

if(NOT MSVC)
    target_compile_options(JuegoEcuaciones PRIVATE -Wall -Wextra)
endif()
