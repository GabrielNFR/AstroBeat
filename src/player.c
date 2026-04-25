#include "player.h"
#include "raymath.h"

void inicializarNave(Nave *jogador)
{
    jogador->posicao = (Vector3){FAIXA_CENTRO, 0.5f, 0.0f};
    // jogador->modelo = LoadModel("assets/InterstellarRunner.vox");
    // jogador->modelo = LoadModel("assets/UltravioletIntruder.vox");
    jogador->modelo = LoadModel("assets/InfraredFurtive.vox");
    // jogador->modelo = LoadModel("assets/Spaceship.glb");

    BoundingBox limites = GetModelBoundingBox(jogador->modelo);
    Vector3 centro = {
        (limites.min.x + limites.max.x) / 2.0f,
        (limites.min.y + limites.max.y) / 2.0f,
        (limites.min.z + limites.max.z) / 2.0f
    };

    Matrix centralizar = MatrixTranslate(-centro.x, -centro.y, -centro.z);

    float fatorReducao = 0.2f; 
    Matrix escala = MatrixScale(fatorReducao, fatorReducao, fatorReducao);
    Matrix giro = MatrixRotateY(180.0f * DEG2RAD); 

    Matrix transformFinal = MatrixMultiply(centralizar, escala);
    transformFinal = MatrixMultiply(transformFinal, giro);

    jogador->modelo.transform = transformFinal;
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
    DrawModel(jogador->modelo, jogador->posicao, 1.0f, WHITE);
   
    // DrawCube(jogador->posicao, 1.0f, 1.0f, 1.0f, RED);
    // DrawCubeWires(jogador->posicao, 1.0f, 1.0f, 1.0f, MAROON); 
}

void descarregarNave(Nave *jogador) {
    UnloadModel(jogador->modelo);
}