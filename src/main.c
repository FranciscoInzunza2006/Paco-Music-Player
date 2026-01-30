#include <stdlib.h>

#include "tracks.h"

#include "music_player.h"
#include "gui.h"

Tracks getTracksFromPaths();

int main(void) {
    const char* MUSIC_DIRECTORIES_PATH[] = {
#if __WIN32__
        //"C:\\Program Files (x86)\\Steam\\steamapps\\music\\Terraria Official Soundtrack",
        //"C:\\Program Files (x86)\\Steam\\steamapps\\music\\OneShot OST",
        //"C:\\Program Files (x86)\\Steam\\steamapps\\music\\OneShot Solstice OST",
        //"C:\\Program Files (x86)\\Steam\\steamapps\\music\\UNDERTALE Soundtrack",
        "C:\\Program Files (x86)\\Steam\\steamapps\\music\\DELTARUNESoundtrack",
        //"D:\\Musica\\OFF",
        //"D:\\Musica\\Ribbit",
#else
        "/home/elpaco/Music/OFF Official Soundtrack"
#endif
    };
    constexpr size_t MUSIC_DIRECTORIES_COUNT = sizeof(MUSIC_DIRECTORIES_PATH) / sizeof(MUSIC_DIRECTORIES_PATH[0]);

    musicPlayer_init(MUSIC_DIRECTORIES_PATH, MUSIC_DIRECTORIES_COUNT);
    GuiPacosState state = guiInit();

    musicPlayer_play();
    while (guiShouldUpdate(&state)) {
        // Update
        musicPlayer_update();
        guiUpdate(&state);
    }
    guiCleanUp(&state);

    //freeTrackList(&all_tracks);
    //freeAlbumList(&albums, false);

    return 0;
}