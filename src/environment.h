#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "raylib.h"
#include "globals.h"

#define QTD_ESTRELAS 100

typedef struct Environment {
    Vector3 posicoes[QTD_ESTRELAS];
    float velocidade;
    float fovAlvo;
    float rotSpeedMult;

    Model sphere;
    float rotacaoSphere;

    Model menuSphere;
    float rotacaoMenuSphere;
}Env;

void inicializarCenario(Env *env);
void atualizarFundo(Env *env, float deltaTime);
void atualizarFundoMenu(Env *env, float deltaTime);
void atualizarCenario(Env *env, float deltaTime, float tempoAtual, int songIndex);
void desenharFundo(Env *env);
void desenharFundoMenu(Env *env);
void desenharPistaEstrelas(Env *env);
void descarregarCenario(Env *env);

#endif
