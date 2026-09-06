project(Editor)

set(SOURCE_DIR src)

# define headers
file(GLOB_RECURSE HEADER_FILES "${SOURCE_DIR}/**.h")

# define source
file(GLOB_RECURSE SOURCE_FILES "${SOURCE_DIR}/**.cpp")

# TODO: remove circular deps with engine
add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

# linking
target_link_libraries(${PROJECT_NAME} Libraries)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE 
        GLM_ENABLE_EXPERIMENTAL
        RESOURCES_PATH="${RESOURCE_DIR}"
)

target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${ENGINE_SRC})
