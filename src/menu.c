#include "menu.h"

// variaveis
static int menuSelectedOption = 0;
static int songSelectedOption = 0;
static const char *mockSongs[] = {
    "Dragonite",
    "Kingambit",
    "Mew"
};
static const int TOTAL_MOCK_SONGS = 3;

// main menu
void atualizarMenu(GameState *state) {
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        menuSelectedOption--;
        if (menuSelectedOption < 0) menuSelectedOption = 0;
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        menuSelectedOption++;
        if (menuSelectedOption > 1) menuSelectedOption = 1;
    }
    if (IsKeyPressed(KEY_ENTER)) {
        if (menuSelectedOption == 0) {
            *state = SONG_SELECT;
            songSelectedOption = 0;
        } else {
            *state = SETTINGS;
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        *state = QUIT;
    }
}

void desenharMenu(void) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    DrawText("AstroBeat", screenWidth/2 - MeasureText("AstroBeat", 40)/2, screenHeight/4, 40, MAGENTA);

    Color opt1Color = (menuSelectedOption == 0) ? MAGENTA : RAYWHITE;
    Color opt2Color = (menuSelectedOption == 1) ? MAGENTA : RAYWHITE;

    DrawText("Lista de Músicas", screenWidth/2 - MeasureText("Lista de Músicas", 20)/2, screenHeight/2, 20, opt1Color);
    DrawText("Configurações", screenWidth/2 - MeasureText("Configurações", 20)/2, screenHeight/2 + 30, 20, opt2Color);

    DrawText("^v para navegar | ENTER para selecionar",
             screenWidth/2 - MeasureText("^v para navegar | ENTER para selecionar", 16)/2,
             screenHeight*3/4, 16, Fade(RAYWHITE, 0.7f));
}

// song select
void atualizarSongSelect(GameState *state) {
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        songSelectedOption--;
        if (songSelectedOption < 0) songSelectedOption = 0;
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        songSelectedOption++;
        if (songSelectedOption >= TOTAL_MOCK_SONGS) songSelectedOption = TOTAL_MOCK_SONGS - 1;
    }
    if (IsKeyPressed(KEY_ENTER)) *state = PLAYING;
    if (IsKeyPressed(KEY_ESCAPE)) *state = MENU;
}
void desenharSongSelect(void) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    DrawText("Lista de Músicas", screenWidth/2 - MeasureText("Lista de Músicas", 30)/2, screenHeight/5, 30, MAGENTA);

    for (int i = 0; i < TOTAL_MOCK_SONGS; i++) {
        Color songColor = (i == songSelectedOption) ? MAGENTA : RAYWHITE;
        DrawText(mockSongs[i], screenWidth/2 - MeasureText(mockSongs[i], 20)/2, screenHeight/3 + i*30, 20, songColor);
    }

    DrawText("^v navegar | ENTER iniciar | ESC voltar",
             screenWidth/2 - MeasureText("^v navegar | ENTER iniciar | ESC voltar", 16)/2,
             screenHeight*3/4, 16, Fade(RAYWHITE, 0.7f));
}

// settings
void atualizarSettings(GameState *state) {
    if (IsKeyPressed(KEY_ESCAPE)) *state = MENU;
}
void desenharSettings(void) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    DrawText("Configurações", screenWidth/2 - MeasureText("Configurações", 30)/2, screenHeight/5, 30, MAGENTA);
    DrawText("Em breve...", screenWidth/2 - MeasureText("Em breve...", 20)/2, screenHeight/2, 20, RAYWHITE);
    DrawText("ESC para voltar", screenWidth/2 - MeasureText("ESC para voltar", 16)/2, screenHeight*3/4, 16, Fade(RAYWHITE, 0.7f));
}
