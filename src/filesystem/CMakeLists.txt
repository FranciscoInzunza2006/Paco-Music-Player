
add_library(filesystem
        filesystem.c
        filesystem.h
)

target_include_directories(filesystem PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})