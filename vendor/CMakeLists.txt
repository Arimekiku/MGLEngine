include(FetchContent)

# dependencies loading process. If you wanna check out dependencies manifest or just lock down a couple of lines
# you'll see checksums, I need them for nix specifically so it can download things
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/dependencies.json" dependency_manifest)

string(JSON dependency_count LENGTH "${dependency_manifest}")
math(EXPR last_dependency "${dependency_count} - 1")

set(dependencies)
foreach(index RANGE ${last_dependency})
    string(JSON dependency MEMBER "${dependency_manifest}" ${index})
    list(APPEND dependencies "${dependency}")

    string(JSON dependency_url GET "${dependency_manifest}" "${dependency}" url)
    string(JSON dependency_sha256 GET "${dependency_manifest}" "${dependency}" sha256)
    set(source_subdir "")

    if(dependency STREQUAL "imgui" OR dependency STREQUAL "imguizmo")
        set(source_subdir SOURCE_SUBDIR "mgl-no-cmake")
    endif()

    FetchContent_Declare(${dependency}
        URL "${dependency_url}"
        URL_HASH "SHA256=${dependency_sha256}"
        DOWNLOAD_EXTRACT_TIMESTAMP FALSE
        ${source_subdir}
    )
endforeach()

# GLFW options
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# SPDLOG options
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SPDLOG_INSTALL OFF CACHE BOOL "" FORCE)
set(SPDLOG_FMT_EXTERNAL OFF CACHE BOOL "" FORCE)
set(SPDLOG_FMT_EXTERNAL_HO OFF CACHE BOOL "" FORCE)
set(SPDLOG_USE_STD_FORMAT OFF CACHE BOOL "" FORCE)

# ASSIMP options
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)

# YAML-CPP options
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(${dependencies})

# ASSIMP forces static MINGW onto its consumers so I keep its winpthread dependency and remain runtime linked
if(MINGW AND NOT BUILD_SHARED_LIBS)
    foreach(link_property IN ITEMS LINK_LIBRARIES INTERFACE_LINK_LIBRARIES)
        get_target_property(assimp_links assimp ${link_property})
        list(REMOVE_ITEM assimp_links -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++)
        set_property(TARGET assimp PROPERTY ${link_property} "${assimp_links}")
    endforeach()
endif()

# GLAD
add_subdirectory(GLAD)

# ImGui
add_library(Libraries STATIC
    "${imgui_SOURCE_DIR}/imgui.cpp"
    "${imgui_SOURCE_DIR}/imgui_draw.cpp"
    "${imgui_SOURCE_DIR}/imgui_tables.cpp"
    "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
    "${imgui_SOURCE_DIR}/imgui_demo.cpp"
    "${imguizmo_SOURCE_DIR}/ImGuizmo.cpp"
)

# linking
target_include_directories(Libraries PUBLIC
    "${imgui_SOURCE_DIR}"
    "${imguizmo_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/stb"
    "${CMAKE_CURRENT_SOURCE_DIR}/pfd"
)

target_link_libraries(Libraries PUBLIC glfw spdlog::spdlog glad glm assimp EnTT::EnTT yaml-cpp)

if(WIN32)
    target_link_libraries(Libraries PUBLIC opengl32)
else()
    target_link_libraries(Libraries PUBLIC GL)
endif()
