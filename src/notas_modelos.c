#include "notas_modelos.h"
#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <rlgl.h>

static void desenharGlow(float x, float z, Color cor)
{
    DrawSphere((Vector3){x, 0.5f, z}, 0.48f, Fade(cor, 0.12f));
    DrawSphere((Vector3){x, 0.5f, z}, 0.36f, Fade(cor, 0.22f));
}

void desenharNotaGrave(float x, float z)
{
    DrawCylinder((Vector3){x, 0.5f, z},  0.01f, 0.35f, 0.35f, 4, BLUE);
    DrawCylinder((Vector3){x, 0.15f, z}, 0.35f, 0.01f, 0.35f, 4, BLUE);
    desenharGlow(x, z, BLUE);
}

void desenharNotaAgudo(float x, float z)
{
    DrawCylinder((Vector3){x, 0.5f, z},  0.01f, 0.35f, 0.35f, 4, RED);
    DrawCylinder((Vector3){x, 0.15f, z}, 0.35f, 0.01f, 0.35f, 4, RED);
    desenharGlow(x, z, RED);
}

void desenharNotaDireita(float x, float z)
{
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

    desenharGlow(x, z, GREEN);
}

void desenharNotaEsquerda(float x, float z)
{
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

    desenharGlow(x, z, YELLOW);
}
