#ifndef NOTAS_MODELOS_H
#define NOTAS_MODELOS_H

#include "notas.h"
#include "raylib.h"

void desenharNotaGrave   (float x, float z);
void desenharNotaAgudo   (float x, float z);
void desenharNotaDireita (float x, float z);
void desenharNotaEsquerda(float x, float z);
void desenharEfeitoAcerto(float x, float z, float tempoDecorrido, float duracaoTotal, Color cor);

#endif