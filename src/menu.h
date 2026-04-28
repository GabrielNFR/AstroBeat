#ifndef MENU_H
#define MENU_H

#include "globals.h"
#include "raylib.h"

void atualizarMenu(GameState *state);
void atualizarSongSelect(GameState *state);
void atualizarSettings(GameState *state);

void desenharMenu(void);
void desenharSongSelect(void);
void desenharSettings(void);

#endif
