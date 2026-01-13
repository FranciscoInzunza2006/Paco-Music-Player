
add_compile_options(-Wall -Wextra -pedantic -Werror)

add_executable(Paco_Music_Player main.c)
target_link_libraries(Paco_Music_Player
        PRIVATE raylib
        PRIVATE tag_c
)

# TODO: Fix hardcoded path
target_include_directories(Paco_Music_Player
        PRIVATE "C:/Projects/Paco_Music_Player/build/_deps/taglib-src/bindings/c"
)