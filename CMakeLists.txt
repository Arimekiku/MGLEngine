cmake_minimum_required(VERSION 3.24)

project(MGLEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Choose the default before dependencies can define it in enumeration order.
option(BUILD_SHARED_LIBS "Build third-party libraries as shared libraries" OFF)

set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/Resources/")
set(ENGINE_SRC "${CMAKE_SOURCE_DIR}/Engine/src")
set(EDITOR_SRC "${CMAKE_SOURCE_DIR}/Editor/src")

add_subdirectory(vendor)

add_subdirectory(Engine)
add_subdirectory(Editor)

target_link_libraries(Engine Editor)
target_link_libraries(Editor Engine)

add_subdirectory(Application)
