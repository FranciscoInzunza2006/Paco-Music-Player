
set(CMAKE_C_STANDARD 23)
add_compile_options(-Wall -Wextra -pedantic -Werror)

add_subdirectory(filesystem)

# Project
add_executable(${PROJECT_NAME} main.c)
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME}
        PRIVATE filesystem
)
