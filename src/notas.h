#ifndef NOTAS_H

#include "player.h"

#define NOTAS_H
#define MAX_NOTAS 1000
#define VEL_NOTAS 10.0f

typedef enum{
    NOTA_GRAVE = 0,
    NOTA_AGUDO = 1,
    NOTA_LONGA = 2,
    NOTA_DIREITA = 3,
    NOTA_ESQUERDA = 4
} Tiponota;

typedef struct{
    float tempo;
    int lane;
    Tiponota tipo;
    int ativa;
    int isHit;
    float duracao;
    int pontos;
}Notas;

extern Notas array_notas[MAX_NOTAS];
extern int total_de_notas;

void leitura_arquivo_musica(void);
void atualizar_notas(float tempo_atual);
void desenhar_notas(float tempo_atual);
void verificarAcertos(Nave *jogador, float tempo_atual);


#endif