#include "environment.h"
#include "raymath.h"
#include "rlgl.h"

void inicializarCenario(Env *env)
{
    env->velocidade = 40.0f;
    env->rotacaoSphere = 0.0f;
    env->rotacaoMenuSphere = 0.0f;

    Mesh sphereMesh = GenMeshSphere(500.0f, 64, 64);
    env->sphere = LoadModelFromMesh(sphereMesh);
    Texture2D texture = LoadTexture("assets/nebulabg2.png");
    SetTextureFilter(texture, TEXTURE_FILTER_TRILINEAR);

    env->sphere.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    env->sphere.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = WHITE;

    Mesh menuSphereMesh = GenMeshSphere(500.0f, 64, 64);
    env->menuSphere = LoadModelFromMesh(menuSphereMesh);
    Texture2D menuTexture = LoadTexture("assets/nebulamenu.png");
    SetTextureFilter(menuTexture, TEXTURE_FILTER_TRILINEAR);

    env->menuSphere.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = menuTexture;
    env->menuSphere.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = WHITE;

    rlDisableBackfaceCulling();
    
     for (int i = 0; i < QTD_ESTRELAS; i++) {
        do {
            env->posicoes[i].x = (float)GetRandomValue(-20, 20);
            env->posicoes[i].y = (float)GetRandomValue(-5, 15);
        } while (sqrt(env->posicoes[i].x * env->posicoes[i].x + (env->posicoes[i].y - 2.0f) * (env->posicoes[i].y - 2.0f)) < 5.0f);

        float espacamentoZ = 60.0f / QTD_ESTRELAS;
        env->posicoes[i].z = -50.0f + (i * espacamentoZ);
        env->posicoes[i].z += (float)GetRandomValue(-10, 10) / 10.0f;
    }
}


void atualizarCenario(Env *env, float deltaTime)
{
    for (int i = 0; i < QTD_ESTRELAS; i++) {
        env->posicoes[i].z += env->velocidade * deltaTime;

        if (env->posicoes[i].z > 10.0f) {
            env->posicoes[i].z -= 60.0f;
            do {
            env->posicoes[i].x = (float)GetRandomValue(-20, 20);
            env->posicoes[i].y = (float)GetRandomValue(-5, 15);
            } while (sqrt(env->posicoes[i].x * env->posicoes[i].x + (env->posicoes[i].y - 2.0f) * (env->posicoes[i].y - 2.0f)) < 5.0f);
        }
    }

    atualizarFundo(env, deltaTime);
}

void atualizarFundo(Env *env, float deltaTime)
{
    env->rotacaoSphere += 5.0f * deltaTime;
    Matrix rotateBase = MatrixRotateX(90.0f * DEG2RAD);
    Matrix rotateGiro = MatrixRotateZ(env->rotacaoSphere * DEG2RAD);
    env->sphere.transform = MatrixMultiply(rotateBase, rotateGiro);
}

void atualizarFundoMenu(Env *env, float deltaTime)
{
    env->rotacaoMenuSphere += 2.0f * deltaTime;

    Matrix rotateBase = MatrixRotateX(90.0f * DEG2RAD);
    Matrix rotateGiro = MatrixRotateZ(env->rotacaoMenuSphere * DEG2RAD);

    env->menuSphere.transform = MatrixMultiply(rotateBase, rotateGiro);
}

void desenharFundo(Env *env)
{
    rlDisableBackfaceCulling();
    DrawModel(env->sphere, (Vector3){0, 0, 0}, 1.0f, WHITE);
    rlEnableBackfaceCulling();
}

void desenharFundoMenu(Env *env)
{
    rlDisableBackfaceCulling();
    DrawModel(env->menuSphere, (Vector3){0, 0, 0}, 1.0f, WHITE);
    rlEnableBackfaceCulling();
}

void desenharPistaEstrelas(Env *env)
{
    for (int i = 0; i < QTD_ESTRELAS; i++) {
        DrawCube(env->posicoes[i], 0.1f, 0.1f, 1.0f, RAYWHITE);
    }

    float tamSegmento = 10.0f;
    int numSegmentos = 10;

    Color neon = (Color){191, 0, 255, 255};
    Color basePista = (Color){15, 5, 25, 255};

    for (int i = 0; i < numSegmentos; i++) {

        float zAtual = 10.0f - (i * tamSegmento) - (tamSegmento / 2.0f);
        float alpha = 1.0f - ((float)i / numSegmentos);

        DrawCube((Vector3){0.0f, -0.1f, zAtual}, LARGURA_PISTA, 0.1f, tamSegmento, Fade(basePista, alpha * 0.9f));
        DrawCube((Vector3){BORDA_ESQUERDA, 0.0f, zAtual}, 0.2f, 0.05f, tamSegmento, Fade(neon, alpha));
        DrawCube((Vector3){BORDA_EC, 0.0f, zAtual}, 0.1f, 0.05f, tamSegmento, Fade(neon, alpha * 0.5f));
        DrawCube((Vector3){BORDA_CD, 0.0f, zAtual}, 0.1f, 0.05f, tamSegmento, Fade(neon, alpha * 0.5f));
        DrawCube((Vector3){BORDA_DIREITA, 0.0f, zAtual}, 0.2f, 0.05f, tamSegmento, Fade(neon, alpha));
    }
}

void descarregarCenario(Env *env)
{
    UnloadTexture(env->sphere.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture);
    UnloadModel(env->sphere);

    UnloadTexture(env->menuSphere.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture);
    UnloadModel(env->menuSphere);
}
