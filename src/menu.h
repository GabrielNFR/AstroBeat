#ifndef MENU_H
#define MENU_H

#include "globals.h"
#include "raylib.h"
#include "audio.h"

void atualizarMenu(GameState *state);
void atualizarSongSelect(GameState *state);
void atualizarSettings(GameState *state, Audio *audio);
void atualizarPause(GameState *state);

void desenharMenu(void);
void desenharSongSelect(void);
void desenharSettings(Audio *audio);
void desenharPause(void);

#endif
