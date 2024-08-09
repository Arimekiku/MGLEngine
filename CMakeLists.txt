cmake_minimum_required(VERSION 3.9.1)

project(RenderingEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(PROJECT_OUTPUT_DIR "${CMAKE_BINARY_DIR}/../bin")
set(PROJECT_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR})
set(PROJECT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/RenderingEngine")
set(PROJECT_LIBRARIES_DIR "${CMAKE_SOURCE_DIR}/vendor")
set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/Resources")

# define headers
file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/*.h ${PROJECT_SOURCE_DIR}/*.hpp ${PROJECT_SOURCE_DIR}/*.ini)

# define executables
file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/*.c)

# set all files
set(AllFile ${HEADER_FILES} ${SOURCE_FILES})
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${AllFile})

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
else ()
    set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")
    add_definitions(-DRELEASE_MODE_ON=0)
endif ()

# GLFW
set(GLFW_DIR "${PROJECT_LIBRARIES_DIR}/GLFW")
add_subdirectory(${GLFW_DIR} EXCLUDE_FROM_ALL)

# GLAD
set(GLAD_DIR "${PROJECT_LIBRARIES_DIR}/GLAD")
add_subdirectory(${GLAD_DIR} EXCLUDE_FROM_ALL)

# glm
set(GLM_DIR "${PROJECT_LIBRARIES_DIR}/glm")
add_subdirectory(${GLM_DIR} EXCLUDE_FROM_ALL)

# spdlog
set(SPDLOG_DIR "${PROJECT_LIBRARIES_DIR}/spdlog")
add_subdirectory(${SPDLOG_DIR} EXCLUDE_FROM_ALL)

# imgui
set(IMGUI_DIR "${PROJECT_LIBRARIES_DIR}/ImGui")
file(GLOB IMGUI_SOURCES
        ${IMGUI_DIR}/**.cpp
        ${IMGUI_DIR}/**.h)
add_library(imgui ${IMGUI_SOURCES})

target_include_directories(imgui PRIVATE ${IMGUI_DIR})
set_target_properties(glfw imgui glad spdlog PROPERTIES FOLDER "vendor")

#################################Executable####################################

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC "${GLFW_INCLUDE_DIRS}" "${SPDLOG_DIR}" "${GLAD_DIR}" "${IMGUI_DIR}" "${GLM_DIR}"
        PRIVATE "${PROJECT_SOURCE_DIR}"
)

target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC glfw opengl32.lib spdlog glad imgui glm
        PRIVATE ${CMAKE_DL_LIBS}
)

target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/RenderingEngine/mxpch.h)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})