cmake_minimum_required(VERSION 4.0)
project(Ray-Tracing-Renderer VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)

add_compile_options(-O3 -Wall)

include(GNUInstallDirs)

file(GLOB_RECURSE SOURCE_FILES source/*.cpp)
file(GLOB_RECURSE HEADER_FILES source/*.h)

add_executable(render)

find_package(OpenMP REQUIRED)
target_link_libraries(render PRIVATE OpenMP::OpenMP_CXX)

find_package(Stb REQUIRED)
target_include_directories(render PRIVATE ${Stb_INCLUDE_DIR})

target_include_directories(render PRIVATE
    ${CMAKE_SOURCE_DIR}/source
    ${CMAKE_SOURCE_DIR}/source/base
    ${CMAKE_SOURCE_DIR}/source/hitable
    ${CMAKE_SOURCE_DIR}/source/material
    ${CMAKE_SOURCE_DIR}/source/texture
)

target_sources(render PRIVATE
    ${SOURCE_FILES}
)

# add_custom_command(
#     TARGET render POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_SOURCE_DIR}/source/images
#     ${CMAKE_BINARY_DIR}/images
#     COMMENT "Copying images folder to build directory"
# )