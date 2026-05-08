#include "raylib.h"
#include "notas.h"
#include "score.h"
#include "buffs.h"
#include <stdio.h>
#include <math.h>

float multiplicador_julgamento(Julgamento nota){
    switch (nota){
    case JULG_OK:         return 1.0f;
    case JULG_GOOD:       return 1.25f;
    case JULG_GREAT:      return 1.5f;
    case JULG_PERFECT:    return 2.0f;

    default:              return 0.0f;
    }
}

float multiplicador_streak(int score){
    if (score>=10) return 2.0f;
    if (score>=20) return 4.0f;
    if (score>=30) return 8.0f;

    return 1.0f;
}

void sistema_pontos(Score * score, Julgamento nota){
    int base = 100;
    score->streak++;

    if (score->streak > score->maior_streak){
        score->maior_streak=score->streak;
    }

    float multjulg= multiplicador_julgamento(nota);

    score->multiplicador= multiplicador_streak(score->streak);

    int pontosganhos= (base * multjulg * score->multiplicador);

    score->pontos += pontosganhos;

    score->ultima_nota = nota;

}

void errar_nota(Score * score){
    score->streak = 0;
    score->multiplicador= 1.0f;
    score->ultima_nota = JULG_MISS;
}

void draw_sistema_pontos(Score score){
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    DrawText(TextFormat("Pontuação: %d", score.pontos),20, 20, 30, WHITE);
    DrawText(TextFormat("%d", score.streak),20, 60, 30, WHITE);
    DrawText(TextFormat("%.0fx", score.multiplicador),20, 100, 30, WHITE);
}