#ifndef GLOBALS_H
#define GLOBALS_H

#define FAIXA_ESQUERDA -2.0f
#define FAIXA_CENTRO 0.0f
#define FAIXA_DIREITA 2.0f

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
