#include <stdio.h>
#include "menu.h"
#include "audio.h"
#include <raylib.h>

// variaveis
static int menuSelectedOption = 0;

// lista de musicas

const SongInfo songList[] = {
    {"Elektronomia", "songs/Elektronomia.ogg", "mapasMusicas/mapaElektronomia.txt", "mapasColetaveis/mapaElektronomia.txt"},
    {"Spektrem", "songs/Spektrem.mp3", "mapasMusicas/mapaSpektrem.txt", "mapasColetaveis/mapaSpektrem.txt"},
    {"DK Invincible", "songs/DKInvincible.mp3", "mapasMusicas/mapaDKInvincible.txt", "mapasColetaveis/mapaDKInvincible.txt"},
    {"Dancefloor Dreamer", "songs/Dancefloor Dreamer.mp3", "mapasMusicas/mapaDancefloorDreamer.txt", NULL},
    {"Teste", NULL, NULL, NULL}
};

int songSelectOption = 0;

const SongInfo* obterMusicaSelecionada(void)
{
    return &songList[songSelectOption];
}

// main menu
void atualizarMenu(GameState *state) {
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        menuSelectedOption--;
        if (menuSelectedOption < 0) menuSelectedOption = 0;
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        menuSelectedOption++;
        if (menuSelectedOption > 2) menuSelectedOption = 2;
    }
    if (IsKeyPressed(KEY_ENTER)) {
        if (menuSelectedOption == 0) {
            *state = SONG_SELECT;
            songSelectOption = 0;
        } else if (menuSelectedOption == 1){
            *state = SETTINGS;
        } else if (menuSelectedOption == 2){
            *state = HIGHSCORES;
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
    Color opt3Color = (menuSelectedOption == 2) ? MAGENTA : RAYWHITE;

    DrawText("Lista de Músicas", screenWidth/2 - MeasureText("Lista de Músicas", 20)/2, screenHeight/2, 20, opt1Color);
    DrawText("Configurações", screenWidth/2 - MeasureText("Configurações", 20)/2, screenHeight/2 + 30, 20, opt2Color);
    DrawText("High Scores", screenWidth/2 - MeasureText("High Scores", 20)/2, screenHeight/2 + 60, 20, opt3Color);

    DrawText("^v para navegar | ENTER para selecionar",
             screenWidth/2 - MeasureText("^v para navegar | ENTER para selecionar", 16)/2,
             screenHeight*3/4, 16, Fade(RAYWHITE, 0.7f));
}

// song select
void atualizarSongSelect(GameState *state) {
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        songSelectOption--;
        if (songSelectOption < 0) songSelectOption = 0;
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        songSelectOption++;
        if (songSelectOption >= TOTAL_SONGS) songSelectOption = TOTAL_SONGS - 1;
    }
    if (IsKeyPressed(KEY_ENTER)) *state = PLAYING;
    if (IsKeyPressed(KEY_ESCAPE)) *state = MENU;
}
void desenharSongSelect(void) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    DrawText("Lista de Músicas", screenWidth/2 - MeasureText("Lista de Músicas", 30)/2, screenHeight/5, 30, MAGENTA);

    for (int i = 0; i < TOTAL_SONGS; i++) {
        Color songColor = (i == songSelectOption) ? MAGENTA : RAYWHITE;
        DrawText(songList[i].nome, screenWidth/2 - MeasureText(songList[i].nome, 20)/2, screenHeight/3 + i*30, 20, songColor);
    }

    DrawText("^v navegar | ENTER iniciar | ESC voltar",
             screenWidth/2 - MeasureText("^v navegar | ENTER iniciar | ESC voltar", 16)/2,
             screenHeight*3/4, 16, Fade(RAYWHITE, 0.7f));
}

// settings
void atualizarSettings(GameState *state, Audio *audio) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        *state = MENU;
    }

    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
        definirVolumeMusica(audio, audio->volumeMusica - 0.05f);
    }

    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
        definirVolumeMusica(audio, audio->volumeMusica + 0.05f);
    }
}

void desenharSettings(Audio *audio) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    char textoVolume[32];
    snprintf(textoVolume, sizeof(textoVolume), "Volume: %.0f%%", audio->volumeMusica * 100.0f);

    DrawText("Configurações", screenWidth/2 - MeasureText("Configurações", 30)/2, screenHeight/5, 30, MAGENTA);
    DrawText(textoVolume, screenWidth/2 - MeasureText(textoVolume, 20)/2, screenHeight/2, 20, RAYWHITE);
    DrawText("A / D ou < / > para ajustar", screenWidth/2 - MeasureText("A / D ou < / > para ajustar", 16)/2, screenHeight/2 + 35, 16, Fade(RAYWHITE, 0.7f));
    DrawText("ESC para voltar", screenWidth/2 - MeasureText("ESC para voltar", 16)/2, screenHeight*3/4, 16, Fade(RAYWHITE, 0.7f));
}

// pause
void atualizarPause(GameState *state) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        *state = PLAYING;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        *state = MENU;
    }

    if (IsKeyPressed(KEY_Q)) {
        *state = QUIT;
    }
}

void desenharPause(void) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    DrawText("PAUSADO", screenWidth/2 - MeasureText("PAUSADO", 40)/2, screenHeight/3, 40, MAGENTA);
    DrawText("ESC - continuar", screenWidth/2 - MeasureText("ESC - continuar", 20)/2, screenHeight/2, 20, RAYWHITE);
    DrawText("ENTER - voltar ao menu", screenWidth/2 - MeasureText("ENTER - voltar ao menu", 20)/2, screenHeight/2 + 30, 20, RAYWHITE);
    DrawText("Q - sair", screenWidth/2 - MeasureText("Q - sair", 20)/2, screenHeight/2 + 60, 20, RAYWHITE);

}
