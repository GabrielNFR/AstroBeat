#include "notas_modelos.h"
#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <rlgl.h>

static Model modeloNotaAzul;
static Model modeloNotaVermelho;
static Model modeloNotaEsquerda;
static Model modeloNotaDireita;
static bool modeloNotaAzulCarregado = false;
static bool modeloNotaVermelhoCarregado = false;
static bool modeloNotaEsquerdaCarregado = false;
static bool modeloNotaDireitaCarregado = false;

//static void desenharGlow(float x, float z, Color cor)
//{
//    DrawSphere((Vector3){x, 0.5f, z}, 0.48f, Fade(cor, 0.12f));
//    DrawSphere((Vector3){x, 0.5f, z}, 0.36f, Fade(cor, 0.22f));
//}

static void prepararModeloNota(Model *modelo)
{
    BoundingBox limites = GetModelBoundingBox(*modelo);
    Vector3 centro = {
        (limites.min.x + limites.max.x) / 2.0f,
        (limites.min.y + limites.max.y) / 2.0f,
        (limites.min.z + limites.max.z) / 2.0f
    };

    Matrix centralizar = MatrixTranslate(-centro.x, -centro.y, -centro.z);
    Matrix escala = MatrixScale(0.085f, 0.085f, 0.085f);

    modelo->transform = MatrixMultiply(centralizar, escala);
}

void carregarModelosNotas(void)
{
    modeloNotaAzul = LoadModel("assets/azulv3.vox");
    modeloNotaAzulCarregado = IsModelValid(modeloNotaAzul);
    if (modeloNotaAzulCarregado) prepararModeloNota(&modeloNotaAzul);

    modeloNotaVermelho = LoadModel("assets/vermelhov3.vox");
    modeloNotaVermelhoCarregado = IsModelValid(modeloNotaVermelho);
    if (modeloNotaVermelhoCarregado) prepararModeloNota(&modeloNotaVermelho);

    modeloNotaEsquerda = LoadModel("assets/esquerda.vox");
    modeloNotaEsquerdaCarregado = IsModelValid(modeloNotaEsquerda);
    if (modeloNotaEsquerdaCarregado) prepararModeloNota(&modeloNotaEsquerda);

    modeloNotaDireita = LoadModel("assets/direita.vox");
    modeloNotaDireitaCarregado = IsModelValid(modeloNotaDireita);
    if (modeloNotaDireitaCarregado) prepararModeloNota(&modeloNotaDireita);
}

void descarregarModelosNotas(void)
{
    if (modeloNotaAzulCarregado)
    {
        UnloadModel(modeloNotaAzul);
        modeloNotaAzulCarregado = false;
    }

    if (modeloNotaVermelhoCarregado)
    {
        UnloadModel(modeloNotaVermelho);
        modeloNotaVermelhoCarregado = false;
    }

    if (modeloNotaEsquerdaCarregado)
    {
        UnloadModel(modeloNotaEsquerda);
        modeloNotaEsquerdaCarregado = false;
    }

    if (modeloNotaDireitaCarregado)
    {
        UnloadModel(modeloNotaDireita);
        modeloNotaDireitaCarregado = false;
    }
}

void desenharNotaGrave(float x, float z)
{
    if (modeloNotaAzulCarregado)
    {
        DrawModelEx(modeloNotaAzul,
                    (Vector3){x, 0.5f, z},
                    (Vector3){0.0f, 1.0f, 0.0f},
                    0.0f,
                    (Vector3){1.0f, 1.0f, 1.0f},
                    WHITE);
    }
    else
    {
        DrawCylinder((Vector3){x, 0.5f, z},  0.01f, 0.35f, 0.35f, 4, BLUE);
        DrawCylinder((Vector3){x, 0.15f, z}, 0.35f, 0.01f, 0.35f, 4, BLUE);
    }

    //desenharGlow(x, z, BLUE);
}

void desenharNotaAgudo(float x, float z)
{
    if (modeloNotaVermelhoCarregado)
    {
        DrawModelEx(modeloNotaVermelho,
                    (Vector3){x, 0.5f, z},
                    (Vector3){0.0f, 1.0f, 0.0f},
                    0.0f,
                    (Vector3){1.0f, 1.0f, 1.0f},
                    WHITE);
    }
    else
    {
        DrawCylinder((Vector3){x, 0.5f, z},  0.01f, 0.35f, 0.35f, 4, RED);
        DrawCylinder((Vector3){x, 0.15f, z}, 0.35f, 0.01f, 0.35f, 4, RED);
    }

    //desenharGlow(x, z, RED);
}

void desenharNotaDireita(float x, float z)
{
    if (modeloNotaDireitaCarregado)
    {
        DrawModelEx(modeloNotaDireita,
                    (Vector3){x, 0.5f, z},
                    (Vector3){0.0f, 1.0f, 0.0f},
                    0.0f,
                    (Vector3){1.0f, 1.0f, 1.0f},
                    WHITE);
        return;
    }

    float raio = 0.18f;
    float comprimentoCorpo = 0.50f;
    float comprimentoPonta = 0.34f;

    rlPushMatrix();
        rlTranslatef(x - comprimentoCorpo / 2.0f, 0.5f, z);
        rlRotatef(-90.0f, 0, 0, 1);
        DrawCylinder((Vector3){0, 0, 0}, raio, raio, comprimentoCorpo, 8, GREEN);
    rlPopMatrix();

    rlPushMatrix();
        rlTranslatef(x + comprimentoCorpo / 2.0f, 0.5f, z);
        rlRotatef(-90.0f, 0, 0, 1);
        DrawCylinder((Vector3){0, 0, 0}, 0.01f, raio, comprimentoPonta, 8, GREEN);
    rlPopMatrix();

    //desenharGlow(x, z, GREEN);
}

void desenharNotaEsquerda(float x, float z)
{
    if (modeloNotaEsquerdaCarregado)
    {
        DrawModelEx(modeloNotaEsquerda,
                    (Vector3){x, 0.5f, z},
                    (Vector3){0.0f, 1.0f, 0.0f},
                    0.0f,
                    (Vector3){1.0f, 1.0f, 1.0f},
                    WHITE);
        return;
    }

    float raio = 0.18f;
    float comprimentoCorpo = 0.50f;
    float comprimentoPonta = 0.34f;

    rlPushMatrix();
        rlTranslatef(x - comprimentoCorpo / 2.0f, 0.5f, z);
        rlRotatef(-90.0f, 0, 0, 1);
        DrawCylinder((Vector3){0, 0, 0}, raio, raio, comprimentoCorpo, 8, YELLOW);
    rlPopMatrix();

    rlPushMatrix();
        rlTranslatef(x - comprimentoCorpo / 2.0f, 0.5f, z);
        rlRotatef(90.0f, 0, 0, 1);                     
        DrawCylinder((Vector3){0, 0, 0}, 0.01f, raio, comprimentoPonta, 8, YELLOW);
    rlPopMatrix();

    //desenharGlow(x, z, YELLOW);
}

void desenharEfeitoAcerto(float x, float z, float tempoDecorrido, float duracaoTotal, Color cor)
{
    float progresso = tempoDecorrido / duracaoTotal;
    if (progresso > 1.0f) progresso = 1.0f;

    float expansaoMaxima = 3.0f;
    float raioAtual = 0.5f + (expansaoMaxima * progresso);

    float opacidade = 1.0f - progresso;
    Color corOnda = Fade(cor, opacidade);

    DrawCylinderWires((Vector3){x, 0.1f, z}, raioAtual, raioAtual, 0.05f, 16, corOnda);
    DrawCylinderWires((Vector3){x, 0.05f, z}, raioAtual * 0.7f, raioAtual * 0.7f, 0.02f, 12, Fade(corOnda, opacidade * 0.5f));
}
