#include "player.h"
#include "raymath.h"
#include "notas.h"

void inicializarNave(Nave *jogador)
{
    jogador->posicao = (Vector3){FAIXA_CENTRO, 0.5f, 0.0f};
    jogador->alvoX = FAIXA_CENTRO;
    jogador->modelo = LoadModel("assets/InfraredFurtive.vox");
    jogador->laneAtual = 1;
    jogador->laneAnterior = 1;

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

    for (int i = 0; i < MAX_PARTICULAS; i++)
    {
        jogador->particulas[i].ativa = false;
    }

    jogador->buffMultiplicador = false;
    jogador->buffJanela = false;
    jogador->tempoFimMult = 0.0f;
    jogador->tempoFimJanela = 0.0f;
}

void atualizarNave(Nave *jogador, float deltaTime)
{
    jogador->laneAnterior = jogador->laneAtual;

    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
            if (jogador->alvoX == FAIXA_CENTRO) jogador->alvoX = FAIXA_ESQUERDA;
            else if (jogador->alvoX == FAIXA_DIREITA) jogador->alvoX = FAIXA_CENTRO;
        }

    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
        if (jogador->alvoX == FAIXA_CENTRO) jogador->alvoX = FAIXA_DIREITA;
        else if (jogador->alvoX == FAIXA_ESQUERDA) jogador->alvoX = FAIXA_CENTRO;
    }

    jogador->posicao.x = Lerp(jogador->posicao.x, jogador->alvoX, 15.0f * deltaTime);

    float direcaoMovimento = jogador->alvoX - jogador->posicao.x;
    float alvoRolamento = direcaoMovimento * 8.0f;

    jogador->anguloRolamento = Lerp(jogador->anguloRolamento, alvoRolamento, 10.0f * deltaTime);

    float diferencaAbsoluta = fabs(jogador->alvoX - jogador->posicao.x);
    jogador->posicao.y = 0.5f + (diferencaAbsoluta * 0.2f);

    jogador->hitbox = (BoundingBox){
        {jogador->posicao.x - 0.4f, jogador->posicao.y - 0.3f, jogador->posicao.z - 0.4f}, 
        {jogador->posicao.x + 0.4f, jogador->posicao.y + 0.3f, jogador->posicao.z + 0.4f} 
    };

    if (jogador->alvoX == FAIXA_ESQUERDA)
    {
        jogador->laneAtual = 0;
    }
    else if (jogador->alvoX == FAIXA_CENTRO)
    {
        jogador->laneAtual = 1;
    }
    else
    {
        jogador->laneAtual = 2;
    }

    // Envelhecer particulas existentes
    for (int i = 0; i < MAX_PARTICULAS; i++)
    {
        if (jogador->particulas[i].ativa)
        {
            jogador->particulas[i].posicao.x += jogador->particulas[i].velocidade.x * deltaTime;
            jogador->particulas[i].posicao.y += jogador->particulas[i].velocidade.y * deltaTime;
            jogador->particulas[i].posicao.z += jogador->particulas[i].velocidade.z * deltaTime;

            jogador->particulas[i].tamanho -= 0.6f * deltaTime;
            jogador->particulas[i].vida -= 2.0f * deltaTime;

            if (jogador->particulas[i].vida <= 0.0f || jogador->particulas[i].tamanho <= 0.0f) {
                jogador->particulas[i].ativa = false;
            }
        }
    }

    // Criar novas particulas para repor as mortas
    int particulasFrame = 0;
    for (int i = 0; i < MAX_PARTICULAS; i++)
    {
        if (!jogador->particulas[i].ativa && particulasFrame < 2)
        {
            jogador->particulas[i].ativa = true;
            jogador->particulas[i].vida = 1.0f;
            jogador->particulas[i].tamanho = (float)GetRandomValue(10, 20) / 100.0f;

            float offsetThrusterX = (particulasFrame == 0) ? -0.3f : 0.3f;

            jogador->particulas[i].posicao.x = jogador->posicao.x + offsetThrusterX + ((float)GetRandomValue(-5, 5) / 100.0f);
            jogador->particulas[i].posicao.y = jogador->posicao.y + 0.1f + ((float)GetRandomValue(-5, 5) / 100.0f);
            jogador->particulas[i].posicao.z = jogador->posicao.z + 0.8f;

            jogador->particulas[i].velocidade = (Vector3){
                (float)GetRandomValue(-5, 5)/10.0f, 
                (float)GetRandomValue(-5, 5)/10.0f, 
                15.0f 
            };

            particulasFrame++;
        }
    }
}

void desenharNave(Nave *jogador)
{
    Vector3 eixoTorcaoZ = {0.0f, 0.0f, 1.0f};
    DrawModelEx(jogador->modelo, jogador->posicao, eixoTorcaoZ, jogador->anguloRolamento, (Vector3){1.0f, 1.0f, 1.0f}, WHITE);

    Vector3 centroHit = { jogador->posicao.x, 0.02f, -HIT_OFFSET };
    Vector3 eixoX      = { 1.0f, 0.0f, 0.0f };  

    DrawCircle3D(centroHit, 0.65f, eixoX, 90.0f, Fade(MAGENTA, 0.25f));
    DrawCircle3D(centroHit, 0.55f, eixoX, 90.0f, Fade(MAGENTA, 0.55f));
    DrawCircle3D(centroHit, 0.50f, eixoX, 90.0f, Fade(MAGENTA, 0.80f));
   
    // DrawCube(jogador->posicao, 1.0f, 1.0f, 1.0f, RED);
    // DrawCubeWires(jogador->posicao, 1.0f, 1.0f, 1.0f, MAROON); 

    for (int i = 0; i < MAX_PARTICULAS; i++)
    {
        if (jogador->particulas[i].ativa)
        {
            Color corFaisca = Fade(MAGENTA, jogador->particulas[i].vida);

            DrawCube(jogador->particulas[i].posicao,
                     jogador->particulas[i].tamanho,
                     jogador->particulas[i].tamanho,
                     jogador->particulas[i].tamanho,
                     corFaisca);
        }
    }
}

void descarregarNave(Nave *jogador) {
    UnloadModel(jogador->modelo);
}