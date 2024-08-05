cmake_minimum_required(VERSION 3.9.1)

project(GLAD)

add_library(glad STATIC
        ${PROJECT_SOURCE_DIR}/include/glad/glad.h
        ${PROJECT_SOURCE_DIR}/include/KHR/khrplatform.h
        ${PROJECT_SOURCE_DIR}/src/glad.c)

target_include_directories(glad PUBLIC
        ${PROJECT_SOURCE_DIR}/include)