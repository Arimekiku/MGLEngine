cmake_minimum_required(VERSION 3.9.1)

project(RenderingEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# hot reload for MSVC compilers if supported
if (MSVC)
    set(CMAKE_CXX_FLAGS "/Zi /EHsc")

    if (POLICY CMP0141)
      cmake_policy(SET CMP0141 NEW)
      set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
    endif()
endif()

set(PROJECT_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/build")
set(PROJECT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(RESOURCE_DIR "./Resources/")

# define headers
file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/**.h)

# define source
file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/**.cpp)

# define resources in binaries
file(COPY ${RESOURCE_DIR} DESTINATION "${CMAKE_BINARY_DIR}/Resources/")

add_subdirectory("vendor" EXCLUDE_FROM_ALL)

################################# Executable ####################################

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${RESOURCE_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}")
target_link_libraries(${PROJECT_NAME} PUBLIC libs PRIVATE ${CMAKE_DL_LIBS})

target_precompile_headers(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/mxpch.h)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})