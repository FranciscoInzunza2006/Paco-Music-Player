
message("\"Filesystem\" module")

add_library(Filesystem
        filesystem.c
        filesystem.h
)
target_include_directories(Filesystem PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})