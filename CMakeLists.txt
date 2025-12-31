cmake_minimum_required(VERSION 3.10)
project(RaeptorCogsShadingLanguage C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    spirv-headers
    GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Headers.git
    GIT_TAG vulkan-sdk-1.4.335.0
)

FetchContent_MakeAvailable(spirv-headers)

FetchContent_Declare(
    spirv-tools
    GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Tools.git
    GIT_TAG v2025.5
)

set(SPIRV_TOOLS_BUILD_STATIC OFF CACHE BOOL "" FORCE)
set(SPIRV_TOOLS_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(SPIRV_TOOLS_BUILD_TOOLS ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(spirv-tools)

FetchContent_Declare(
    glslang
    GIT_REPOSITORY https://github.com/KhronosGroup/glslang.git
    GIT_TAG 16.1.0
)

set(ENABLE_OPT ON CACHE BOOL "" FORCE)
set(ENABLE_SPIRV_TOOLS ON CACHE BOOL "" FORCE)
set(ENABLE_HLSL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glslang)

# Source files
file(GLOB_RECURSE SOURCES "src/*.c")

# Remove all main.c files from sources
foreach(FILE_PATH ${SOURCES})
    get_filename_component(FILE_NAME ${FILE_PATH} NAME)
    if(FILE_NAME STREQUAL "main.c")
        list(REMOVE_ITEM SOURCES ${FILE_PATH})
    endif()
endforeach()

# Header directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# External dependencies
include_directories(${PROJECT_SOURCE_DIR}/external)

# argparse
list(APPEND SOURCES
    ${PROJECT_SOURCE_DIR}/external/argparse/argparse.c
)

# GLSLang C interface
list(APPEND SOURCES
    ${PROJECT_SOURCE_DIR}/src/external/glslang_c.cpp
)

# Function for adding an executable with common settings
function(add_rgsl_executable target_name)
    add_executable(${target_name} ${ARGN})
    target_include_directories(${target_name} PRIVATE ${PROJECT_SOURCE_DIR}/include)
    target_link_libraries(${target_name} PRIVATE glslang)

    # Optional: Common compile options
    if(CMAKE_C_COMPILER_ID MATCHES "Clang")
        #target_compile_options(${target_name} PRIVATE -Weverything)
    elseif(CMAKE_C_COMPILER_ID MATCHES "GNU")
        target_compile_options(${target_name} PRIVATE -Weverything)
    else()
        target_compile_options(${target_name} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

function(disable_warnings TARGET_NAME)
    if (TARGET ${TARGET_NAME})
        if (MSVC)
            target_compile_options(${TARGET_NAME} PRIVATE
                /W0
                /clang:-Wno-everything
            )
        elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            target_compile_options(${TARGET_NAME} PRIVATE
                -Wno-everything
            )
        elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            target_compile_options(${TARGET_NAME} PRIVATE
                -w
            )
        elseif(MSVC)
            target_compile_options(${TARGET_NAME} PRIVATE
                /W0
            )
        endif()
    endif()
endfunction()


# Add the main RGSL executables
add_rgsl_executable(rgsl src/main.c ${SOURCES})

disable_warnings(spirv-headers)
disable_warnings(spirv-tools)
disable_warnings(glslang)