#ifndef SCORE_H
#define SCORE_H

#include "globals.h"



void sistema_pontos(Score * score, Julgamento nota, int pontosBase);
void errar_nota(Score * score);
void draw_sistema_pontos(Score *score);
void init_sistema_pontos(Score *score);


#endif
