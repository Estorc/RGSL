cmake_minimum_required(VERSION 3.10)
project(RaeptorCogsShadingLanguage C)

set(CMAKE_C_STANDARD 99)
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

set(ENABLE_GLSLANG_BINARIES ON CACHE BOOL "" FORCE)
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

# Function for adding an executable with common settings
function(add_rgsl_executable target_name)
    add_executable(${target_name} ${ARGN})
    target_include_directories(${target_name} PRIVATE ${PROJECT_SOURCE_DIR}/include)

    # Optional: Common compile options
    if(CMAKE_C_COMPILER_ID MATCHES "Clang")
        #target_compile_options(${target_name} PRIVATE -Weverything)
    elseif(CMAKE_C_COMPILER_ID MATCHES "GNU")
        target_compile_options(${target_name} PRIVATE -Weverything)
    else()
        target_compile_options(${target_name} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()


set(GLSLANG_TOOLS_DIR "${glslang_BINARY_DIR}/StandAlone")
set(SPIRV_TOOLS_DIR "${spirv-tools_BINARY_DIR}/tools")

set(GLSLANG_VALIDATOR "${GLSLANG_TOOLS_DIR}/glslangValidator")
set(SPIRV_VAL "${SPIRV_TOOLS_DIR}/spirv-val")
set(SPIRV_OPT "${SPIRV_TOOLS_DIR}/spirv-opt")
if (WIN32)
    string(APPEND GLSLANG_VALIDATOR ".exe")
    string(APPEND SPIRV_VAL ".exe")
    string(APPEND SPIRV_OPT ".exe")
endif()

function(copy_tool tool_path target)
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${tool_path}
            $<TARGET_FILE_DIR:${target}>
    )
endfunction()


# Add the main RGSL executables
add_rgsl_executable(RGSL src/main.c ${SOURCES})

copy_tool(${GLSLANG_VALIDATOR} RGSL)
copy_tool(${SPIRV_VAL} RGSL)
copy_tool(${SPIRV_OPT} RGSL)