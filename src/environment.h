#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "raylib.h"
#include "globals.h"

#define QTD_ESTRELAS 100

typedef struct Environment {
    Vector3 posicoes[QTD_ESTRELAS];
    float velocidade;
    Model sphere;
    float rotacaoSphere;
}Env;

void inicializarCenario(Env *env);
void atualizarCenario(Env *env, float deltaTime);
void desenharFundo(Env *env);
void desenharPistaEstrelas(Env *env);
void descarregarCenario(Env *env);

#endif