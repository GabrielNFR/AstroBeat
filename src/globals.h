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

#endif
