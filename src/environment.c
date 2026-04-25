#include "environment.h"

void inicializarCenario(Env *env)
{
    env->velocidade = 40.0f;
    env->texture = LoadTexture("assets/nebulabg1.png");

     for (int i = 0; i < QTD_ESTRELAS; i++) {
        do {
            env->posicoes[i].x = (float)GetRandomValue(-20, 20);
            env->posicoes[i].y = (float)GetRandomValue(-5, 15);
        } while (env->posicoes[i].x > -4.0f && env->posicoes[i].x < 4.0f && env->posicoes[i].y > -1.0f && env->posicoes[i].y < 5.0f);

        env->posicoes[i].z = (float)GetRandomValue(-50, 10);
    }
}

void atualizarCenario(Env *env, float deltaTime)
{
    for (int i = 0; i < QTD_ESTRELAS; i++) {
        env->posicoes[i].z += env->velocidade * deltaTime;

        // Se a estrela passou da câmera (Z > 10), ela "renasce" lá no fundo (Z = -50)
        if (env->posicoes[i].z > 10.0f) {
            env->posicoes[i].z = -50.0f;
            // Sorteia nova posição horizontal e vertical para não ficar padronizado
            do {
            env->posicoes[i].x = (float)GetRandomValue(-20, 20);
            env->posicoes[i].y = (float)GetRandomValue(-5, 15);
            } while (env->posicoes[i].x > -4.0f && env->posicoes[i].x < 4.0f && env->posicoes[i].y > -1.0f && env->posicoes[i].y < 5.0f);
        }
    }
}

void desenharFundo(Env *env)
{
    Rectangle sourceRec = {0.0f, 0.0f, (float)env->texture.width, (float)env->texture.height};
    Rectangle destRec = {0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()};
    Vector2 origin = {0.0f, 0.0f};
    DrawTexturePro(env->texture, sourceRec, destRec, origin, 0.0f, WHITE);
}

void desenharPistaEstrelas(Env *env)
{
    for (int i = 0; i < QTD_ESTRELAS; i++) {
        DrawCube(env->posicoes[i], 0.1f, 0.1f, 1.0f, RAYWHITE); 
    }

    float comprimentoPista = 100.0f;
    float larguraPista = 6.0f;
    Vector3 meioPista = {0.0f, -0.1f, -comprimentoPista/2.0f + 10.0f};

    DrawCube(meioPista, larguraPista, 0.1f, comprimentoPista, Fade(DARKGRAY, 0.8f));
    Color neon = (Color){0, 228, 255, 255};

    DrawCube((Vector3){-3.0f, 0.0f, meioPista.z}, 0.2f, 0.05f, comprimentoPista, neon);
    DrawCube((Vector3){-1.0f, 0.0f, meioPista.z}, 0.1f, 0.05f, comprimentoPista, Fade(neon, 0.5f));
    DrawCube((Vector3){1.0f, 0.0f, meioPista.z}, 0.1f, 0.05f, comprimentoPista, Fade(neon, 0.5f));
    DrawCube((Vector3){3.0f, 0.0f, meioPista.z}, 0.2f, 0.05f, comprimentoPista, neon);
}

void descarregarCenario(Env *env)
{
    UnloadTexture(env->texture);
}