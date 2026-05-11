#ifndef GLOBALS_H
#define GLOBALS_H

#define FAIXA_ESQUERDA -2.8f
#define FAIXA_CENTRO 0.0f
#define FAIXA_DIREITA 2.8f
#define BORDA_ESQUERDA (FAIXA_ESQUERDA - 1.0f)
#define BORDA_EC ((FAIXA_ESQUERDA + FAIXA_CENTRO) / 2.0f)
#define BORDA_CD ((FAIXA_CENTRO + FAIXA_DIREITA) / 2.0f)
#define BORDA_DIREITA (FAIXA_DIREITA + 1.0f)
#define LARGURA_PISTA (BORDA_DIREITA - BORDA_ESQUERDA)

typedef enum {
    MENU,
    SONG_SELECT,
    SETTINGS,
    PLAYING,
    PAUSED,
    QUIT
} GameState;
extern GameState gameState;

typedef enum{
    NOTA_GRAVE = 0,
    NOTA_AGUDO = 1,
    NOTA_LONGA = 2,
    NOTA_DIREITA = 3,
    NOTA_ESQUERDA = 4
} Tiponota;

typedef enum{
    JULG_PENDENTE = 0,
    JULG_PERFECT,
    JULG_GREAT,
    JULG_GOOD,
    JULG_OK,
    JULG_MISS
}Julgamento;

typedef struct {
    int pontos;
    int streak;
    int maior_streak;
    float multiplicador;
    Julgamento ultima_nota;
}Score;

#endif
