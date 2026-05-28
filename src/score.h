#ifndef SCORE_H
#define SCORE_H

#include "globals.h"

extern const char *nomesColunas[TOTAL_COLUNAS];

void aumentar_streak(Score *score,int valor);
void adicionar_pontos(Score *score,Julgamento nota,int pontosBase);
void errar_nota(Score * score);
void draw_sistema_pontos(Score *score);
void init_sistema_pontos(Score *score);
void draw_Resultados(Score *score);
void atualizarResultados(GameState *gameState);
void calcularTotais(Score *score);
void registrarAcerto(Score *score,Tiponota tipo);
void registrarmiss(Score *score,Tiponota tipo);
void calcularPrecisao(Score *score);


#endif
