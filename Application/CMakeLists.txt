add_executable(MGLEngine main.cpp)

target_link_libraries(MGLEngine Editor)
target_link_libraries(MGLEngine Engine)

target_compile_definitions(MGLEngine PRIVATE _CRT_SECURE_NO_WARNINGS)

target_include_directories(${PROJECT_NAME} PRIVATE ${EDITOR_SRC})
target_include_directories(${PROJECT_NAME} PRIVATE ${ENGINE_SRC})