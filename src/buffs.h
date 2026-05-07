#ifndef BUFFS_H
#define BUFFS_H

#include "raylib.h"

typedef struct Nave Nave;

typedef enum TipoBuff
{
    BUFF_MULTIPLICADOR = 0,
    BUFF_JANELA = 1
}TipoBuff;

typedef struct ListaColetaveis {
    Vector3 posicao;
    TipoBuff tipobuff;
    float duracao;
    float tempoSpawn;
    int lane;
    int ativo;
    struct ListaColetaveis *proximo;
}Coletavel;

extern Coletavel *listaColetaveis;

void inserirColetavel(float tempo, int lane, TipoBuff tipobuff, float duracao);
void removerColetavel(Coletavel *anterior, Coletavel *atual);
void atualizarColetavel(float tempo_atual);
void desenharColetavel(float tempo_atual);
void verificarColisao(Nave *jogador, float tempo_atual);
void limparColetavel(void);

#endif