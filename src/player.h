#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "globals.h"

typedef struct {
    Vector3 posicao;
    Model modelo;
}Nave;

void inicializarNave(Nave *jogador);
void atualizarNave(Nave *jogador);
void desenharNave(Nave *jogador);
void descarregarNave(Nave *jogador);

#endif