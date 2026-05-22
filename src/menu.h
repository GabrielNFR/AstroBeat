#ifndef MENU_H
#define MENU_H

#include "globals.h"
#include "raylib.h"
#include "audio.h"

#define TOTAL_SONGS 5

typedef struct {
    const char *nome;
    const char *caminhoMusica;
    const char *caminhoBeatmap;
    const char *caminhoColetaveis;
}SongInfo;

extern const SongInfo songList[];
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
