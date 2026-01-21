
add_compile_options(-Wall -Wextra -pedantic -Werror -Wno-unused-parameter)

add_library(DynamicArray INTERFACE dynamic_array.h)

add_library(Tracks STATIC tracks.h tracks.c)
target_link_libraries(Tracks
        PRIVATE DynamicArray
        PRIVATE raylib
        PRIVATE tag_c
)
target_include_directories(Tracks
        PRIVATE ${taglib_SOURCE_DIR}/bindings/c
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(MusicPlayer STATIC music_player.h music_player.c)
target_link_libraries(MusicPlayer PRIVATE raylib)

add_library(GUI STATIC interface.c interface.h)
target_link_libraries(GUI
        PRIVATE raylib
        PRIVATE raygui
        PRIVATE MusicPlayer
)

add_executable(Paco_Music_Player main.c)
target_link_libraries(Paco_Music_Player
        PRIVATE raylib
        PRIVATE Tracks
        PRIVATE MusicPlayer
        PRIVATE GUI
)

