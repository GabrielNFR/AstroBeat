#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "globals.h"

#define MAX_PARTICULAS 60

typedef struct {
    Vector3 posicao;
    Vector3 velocidade;
    float tamanho;
    float vida;
    bool ativa;
}Particula;

typedef struct Nave{
    Vector3 posicao;
    BoundingBox hitbox;
    Model modelo;
    Particula particulas[MAX_PARTICULAS];
    float alvoX;
    float anguloRolamento;
    float laneAtual;
    float laneAnterior;
    float tempoFimMult;
    float tempoFimJanela;
    bool buffMultiplicador;
    bool buffJanela;
}Nave;

void inicializarNave(Nave *jogador);
void atualizarNave(Nave *jogador, float deltaTime);
void desenharNave(Nave *jogador);
void descarregarNave(Nave *jogador);

#endif