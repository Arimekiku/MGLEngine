cmake_minimum_required(VERSION 3.9.1)

project(libs)

# GLFW
set(GLFW_DIR "GLFW")
add_subdirectory(${GLFW_DIR} EXCLUDE_FROM_ALL)

# GLAD
set(GLAD_DIR "GLAD")
add_subdirectory(${GLAD_DIR} EXCLUDE_FROM_ALL)

# glm
set(GLM_DIR "glm")
add_subdirectory(${GLM_DIR} EXCLUDE_FROM_ALL)

# spdlog
set(SPDLOG_DIR "spdlog")
add_subdirectory(${SPDLOG_DIR} EXCLUDE_FROM_ALL)

# imgui
set(IMGUI_DIR "ImGui")
file(GLOB IMGUI_SOURCES ${IMGUI_DIR}/**.cpp ${IMGUI_DIR}/**.h)

#stb
set(STB_DIR "stb")
file(GLOB STB_SOURCES ${STB_DIR}/stb_image.h)

#assimp
set(ASSIMP_DIR "assimp")
set(ASSIMP_WARNINGS_AS_ERRORS OFF)
add_subdirectory(${ASSIMP_DIR} EXCLUDE_FROM_ALL)

#imguizmo
set(IMGUIZMO_DIR "ImGuizmo")
file(GLOB IMGUIZMO_SOURCES ${IMGUIZMO_DIR}/ImGuizmo.h ${IMGUIZMO_DIR}/ImGuizmo.cpp)

#entt
set(ENTT_DIR "entt")
add_subdirectory(${ENTT_DIR} EXCLUDE_FROM_ALL)

#yaml-cpp
set(YAMLCPP_DIR "yaml-cpp")
add_subdirectory(${YAMLCPP_DIR} EXCLUDE_FROM_ALL)

set(PFD_DIR "pfd")
file(GLOB PFD_SOURCES ${PFD_DIR}/portable_file_dialogs.h)

################################# Executable ####################################

add_library(${PROJECT_NAME} ${IMGUI_SOURCES} ${STB_SOURCES} ${IMGUIZMO_SOURCES} ${PFD_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${PROJECT_NAME} PUBLIC 
    "${GLFW_INCLUDE_DIRS}" 
    "${SPDLOG_DIR}" 
    "${GLAD_DIR}" 
    "${IMGUI_DIR}" 
    "${GLM_DIR}" 
    "${STB_DIR}"
    "${ASSIMP_DIR}" 
    "${IMGUIZMO_DIR}" 
    "${ENTT_DIR}"
    "${YAMLCPP_DIR}"
    "${PFD_DIR}")

target_link_libraries(${PROJECT_NAME} PUBLIC glfw spdlog glad glm assimp EnTT yaml-cpp)

if(WIN32)
        target_link_libraries(${PROJECT_NAME} PUBLIC opengl32)
else()
        target_link_libraries(${PROJECT_NAME} PUBLIC GL)
endif()
