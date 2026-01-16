
add_compile_options(-Wall -Wextra -pedantic -Werror -Wno-unused-parameter)

add_executable(Paco_Music_Player main.c)
target_link_libraries(Paco_Music_Player
        PRIVATE raylib
        PRIVATE raygui
        PRIVATE tag_c
)

target_include_directories(Paco_Music_Player
        PRIVATE ${taglib_SOURCE_DIR}/bindings/c
)