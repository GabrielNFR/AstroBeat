#ifndef NOTAS_H

#include "player.h"

#define NOTAS_H
#define MAX_NOTAS 1000
#define VEL_NOTAS 10.0f
#define TEMPO_ATE_HIT (80.0f / VEL_NOTAS)
#define JANELA_PERFECT   0.050f
#define JANELA_GREAT     0.075f
#define JANELA_GOOD      0.100f
#define JANELA_OK        0.125f
#define JANELA_MISS      0.150f

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

typedef struct{
    float tempo;
    float duracao;
    float consumo;
    Tiponota tipo;
    Julgamento resultado;
    int lane;
    int ativa;
    int finalizada;
    int pontos;
    int holding;
}Notas;

extern Notas array_notas[MAX_NOTAS];
extern int total_de_notas;

void leitura_arquivo_musica(void);
void atualizar_notas(float tempo_atual);
void desenhar_notas(float tempo_atual);
void resetar_notas(void);
void verificarAcertos(Nave *jogador, float tempo_atual, float deltaTime);

#endif
