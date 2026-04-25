#include "player.h"

void inicializarNave(Nave *jogador)
{
    jogador->posicao = (Vector3){FAIXA_CENTRO, 0.5f, 0.0f};
}

void atualizarNave(Nave *jogador)
{
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
            if (jogador->posicao.x == FAIXA_CENTRO) jogador->posicao.x = FAIXA_ESQUERDA;
            else if (jogador->posicao.x == FAIXA_DIREITA) jogador->posicao.x = FAIXA_CENTRO;
        }

    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
        if (jogador->posicao.x == FAIXA_CENTRO) jogador->posicao.x = FAIXA_DIREITA;
        else if (jogador->posicao.x == FAIXA_ESQUERDA) jogador->posicao.x = FAIXA_CENTRO;
    }
}

void desenharNave(Nave *jogador)
{
    DrawCube(jogador->posicao, 1.0f, 1.0f, 1.0f, RED);
    DrawCubeWires(jogador->posicao, 1.0f, 1.0f, 1.0f, MAROON); 
}