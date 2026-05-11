#ifndef MENU_H
#define MENU_H

#include "globals.h"
#include "raylib.h"
#include "audio.h"

typedef struct {
    const char *nome;
    const char *caminhoMusica;
    const char *caminhoBeatmap;
    const char *caminhoColetaveis;
}SongInfo;

extern const SongInfo songList[];
extern const int TOTAL_SONGS;
extern int songSelectOption;

const SongInfo* obterMusicaSelecionada(void);

void atualizarMenu(GameState *state);
void atualizarSongSelect(GameState *state);
void atualizarSettings(GameState *state, Audio *audio);
void atualizarPause(GameState *state);

void desenharMenu(void);
void desenharSongSelect(void);
void desenharSettings(Audio *audio);
void desenharPause(void);

#endif
