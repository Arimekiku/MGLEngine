cmake_minimum_required(VERSION 3.9.1)

project(RenderingEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -std=c++0x -static -static-libstdc++ -static-libgcc -Wno-error=unknown-pragmas")

set(PROJECT_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin")
set(PROJECT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(PROJECT_LIBRARIES_DIR "${CMAKE_SOURCE_DIR}/vendor")
set(RESOURCE_DIR "${PROJECT_SOURCE_DIR}/Editor/Resources")

# define headers
file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/*.h)

# define executables
file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.cpp)

# define resources in binaries
file(COPY ${RESOURCE_DIR} DESTINATION ${PROJECT_OUTPUT_DIR})

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

#stb
set(STB_DIR "${PROJECT_LIBRARIES_DIR}/stb")
file(GLOB STB_SOURCES
        ${STB_DIR}/**.cpp
        ${STB_DIR}/**.h)
add_library(stb ${IMGUI_SOURCES})

#assimp
set(ASSIMP_DIR "${PROJECT_LIBRARIES_DIR}/assimp")
add_subdirectory(${ASSIMP_DIR} EXCLUDE_FROM_ALL)

target_include_directories(imgui PRIVATE ${IMGUI_DIR})
set_target_properties(glfw imgui glad spdlog stb assimp PROPERTIES FOLDER "vendor")

################################# Executable ####################################

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${PROJECT_SOURCE_DIR}/Editor/Resources/")
else()
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./Resources/")
endif()

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC "${GLFW_INCLUDE_DIRS}" "${SPDLOG_DIR}" "${GLAD_DIR}" "${IMGUI_DIR}" "${GLM_DIR}" "${STB_DIR}" "${ASSIMP_DIR}"
        PRIVATE "${PROJECT_SOURCE_DIR}"
)

target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC glfw opengl32.lib spdlog glad imgui glm stb assimp
        PRIVATE ${CMAKE_DL_LIBS}
)

target_precompile_headers(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/mxpch.h)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})