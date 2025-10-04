cmake_minimum_required(VERSION 3.15)

project(MGLEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/Resources/")
set(ENGINE_SRC "${CMAKE_SOURCE_DIR}/Engine/src")
set(EDITOR_SRC "${CMAKE_SOURCE_DIR}/Editor/src")

add_subdirectory(vendor)

add_subdirectory(Engine)
add_subdirectory(Editor)

add_subdirectory(Application)