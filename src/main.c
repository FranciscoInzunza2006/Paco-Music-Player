#include <stdlib.h>

#include "tracks.h"

#include "music_player.h"
#include "interface.h"

Tracks getTracksFromPaths();

int main(void) {
    const char* MUSIC_DIRECTORIES_PATH[] = {
        //"C:\\Program Files (x86)\\Steam\\steamapps\\music\\Terraria Official Soundtrack",
        //"C:\\Program Files (x86)\\Steam\\steamapps\\music\\OneShot OST",
        //"C:\\Program Files (x86)\\Steam\\steamapps\\music\\OneShot Solstice OST",
        "C:\\Program Files (x86)\\Steam\\steamapps\\music\\UNDERTALE Soundtrack",
        //"C:\\Program Files (x86)\\Steam\\steamapps\\music\\DELTARUNESoundtrack",
        //"D:\\Musica\\OFF",
        //"D:\\Musica\\Ribbit",
    };
    constexpr size_t MUSIC_DIRECTORIES_COUNT = _countof(MUSIC_DIRECTORIES_PATH);

    musicPlayer_init(MUSIC_DIRECTORIES_PATH, MUSIC_DIRECTORIES_COUNT);
    GuiLayoutState state = interfaceInit();

    musicPlayer_play();
    while (interfaceShouldUpdate()) {
        // Update
        musicPlayer_update();
        interfaceUpdate(&state);

        // Draw

    }
    interfaceCleanUp();

    //freeTrackList(&all_tracks);
    //freeAlbumList(&albums, false);

    return 0;
}