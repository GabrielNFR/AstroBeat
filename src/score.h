#ifndef SCORE_H
#define SCORE_H

typedef struct {
    int pontos;
    int streak;
    int maior_streak;
    float multiplicador;
    Julgamento ultima_nota;
}Score;

void sistema_pontos(Score * score, Julgamento nota);
void errar_nota(Score * score);
void draw_sistema_pontos(Score score);


#endif
