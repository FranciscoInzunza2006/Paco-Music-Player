cmake_minimum_required(VERSION 4.1)
project(Paco_Music_Player C)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#region Dependencies
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
set(BUILD_EXAMPLES OFF CACHE BOOL "")
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)

        FetchContent_MakeAvailable(raylib)
    endif()
endif()
#endregion

# Project
set(CMAKE_C_STANDARD 23)

add_library(filesystem src/filesystem/filesystem.c)
add_compile_options(-Wall -Wextra -pedantic -Werror)

add_executable(${PROJECT_NAME} src/main.c)
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} filesystem)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
