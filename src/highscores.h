#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#include "globals.h"

#define ARQUIVO_HIGHSCORES "highscores.cfg"

void salvarHighscore(int songIndex, int pontos);
void carregarHighscores(void);
int obterHighscore(int songIndex);
void desenharHighscores(void);
void atualizarHighscores(GameState *state);

#endif