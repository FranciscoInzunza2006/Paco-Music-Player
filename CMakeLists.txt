cmake_minimum_required(VERSION 4.1)
project(Paco_Music_Player C CXX)

set(CMAKE_C_STANDARD 23)

## Dependencies
include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
set(BUILD_EXAMPLES OFF CACHE BOOL "")

# Raylib
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED

if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)

        FetchContent_MakeAvailable(raylib)
        target_compile_definitions(raylib
                PRIVATE SUPPORT_FILEFORMAT_FLAC=1
        )
    endif ()
endif ()

# RayGUI
FetchContent_Declare(
        raygui
        URL https://github.com/raysan5/raygui/archive/refs/tags/4.0.tar.gz
)
FetchContent_MakeAvailable(raygui)
add_subdirectory(${raygui_SOURCE_DIR}/projects/CMake)

# Taglib
set(BUILD_TESTING OFF)
FetchContent_Declare(
        taglib
        URL https://taglib.org/releases/taglib-2.1.1.tar.gz
)
FetchContent_MakeAvailable(taglib)

add_subdirectory(src)

