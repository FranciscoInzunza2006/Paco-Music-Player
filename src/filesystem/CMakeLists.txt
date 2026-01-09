
message("\"Filesystem\" module")

add_library(Filesystem SHARED
        filesystem.c
        filesystem.h
)
target_include_directories(Filesystem PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(Filesystem PRIVATE FILESYSTEM_EXPORTS)