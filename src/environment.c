#include "environment.h"
#include "raymath.h"
#include "rlgl.h"

void inicializarCenario(Env *env)
{
    env->velocidade = 40.0f;
    env->rotacaoSphere = 0.0f;

    Mesh sphereMesh = GenMeshSphere(500.0f, 64, 64);
    env->sphere = LoadModelFromMesh(sphereMesh);
    Texture2D texture = LoadTexture("assets/nebulabg1.png");
    SetTextureFilter(texture, TEXTURE_FILTER_TRILINEAR);

    env->sphere.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    env->sphere.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = WHITE;
    rlDisableBackfaceCulling();

     for (int i = 0; i < QTD_ESTRELAS; i++) {
        do {
            env->posicoes[i].x = (float)GetRandomValue(-20, 20);
            env->posicoes[i].y = (float)GetRandomValue(-5, 15);
        } while (env->posicoes[i].x > -4.0f && env->posicoes[i].x < 4.0f && env->posicoes[i].y > -1.0f && env->posicoes[i].y < 5.0f);

        float espacamentoZ = 60.0f / QTD_ESTRELAS;
        env->posicoes[i].z = -50.0f + (i * espacamentoZ);
        env->posicoes[i].z += (float)GetRandomValue(-10, 10) / 10.0f; 
    }
}

void atualizarCenario(Env *env, float deltaTime)
{
    for (int i = 0; i < QTD_ESTRELAS; i++) {
        env->posicoes[i].z += env->velocidade * deltaTime;

        // Se a estrela passou da câmera (Z > 10), ela "renasce" lá no fundo (Z = -50)
        if (env->posicoes[i].z > 10.0f) {
            env->posicoes[i].z -= 60.0f;
            // Sorteia nova posição horizontal e vertical para não ficar padronizado
            do {
            env->posicoes[i].x = (float)GetRandomValue(-20, 20);
            env->posicoes[i].y = (float)GetRandomValue(-5, 15);
            } while (env->posicoes[i].x > -4.0f && env->posicoes[i].x < 4.0f && env->posicoes[i].y > -1.0f && env->posicoes[i].y < 5.0f);
        }
    }

    env->rotacaoSphere += 1.0f * deltaTime;
    Matrix rotateBase = MatrixRotateX(90.0f * DEG2RAD);
    Matrix rotateGiro = MatrixRotateZ(env->rotacaoSphere * DEG2RAD);
    env->sphere.transform = MatrixMultiply(rotateBase, rotateGiro);
}

void desenharPistaEstrelas(Env *env)
{
    rlDisableBackfaceCulling();
    DrawModel(env->sphere, (Vector3){0, 0, 0}, 1.0f, WHITE);
    rlEnableBackfaceCulling();
    
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
    UnloadTexture(env->sphere.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture);
    UnloadModel(env->sphere);
}