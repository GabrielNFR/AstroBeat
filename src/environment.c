#include "environment.h"
#include "raymath.h"
#include "rlgl.h"

void inicializarCenario(Env *env)
{
    env->velocidade = 40.0f;
    env->rotacaoSphere = 0.0f;

    Mesh sphereMesh = GenMeshSphere(500.0f, 64, 64);
    env->sphere = LoadModelFromMesh(sphereMesh);
    Texture2D texture = LoadTexture("assets/nebulabg2.png");
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

    env->rotacaoSphere += 20.0f * deltaTime;
    Matrix rotateBase = MatrixRotateX(90.0f * DEG2RAD);
    Matrix rotateGiro = MatrixRotateZ(env->rotacaoSphere * DEG2RAD);
    env->sphere.transform = MatrixMultiply(rotateBase, rotateGiro);
}

void desenharFundo(Env *env)
{
    rlDisableBackfaceCulling();
    DrawModel(env->sphere, (Vector3){0, 0, 0}, 1.0f, WHITE);
    rlEnableBackfaceCulling();
}

void desenharPistaEstrelas(Env *env)
{  
    for (int i = 0; i < QTD_ESTRELAS; i++) {
        DrawCube(env->posicoes[i], 0.1f, 0.1f, 1.0f, RAYWHITE); 
    }

    float tamSegmento = 10.0f;
    int numSegmentos = 10;
    float larguraPista = 6.0f;
    
    Color neon = (Color){0, 228, 255, 255};
    Color basePista = (Color){15, 5, 25, 255}; 

    for (int i = 0; i < numSegmentos; i++) {

        float zAtual = 10.0f - (i * tamSegmento) - (tamSegmento / 2.0f);      
        float alpha = 1.0f - ((float)i / numSegmentos); 
        
        DrawCube((Vector3){0.0f, -0.1f, zAtual}, larguraPista, 0.1f, tamSegmento, Fade(basePista, alpha * 0.9f));
        DrawCube((Vector3){-3.0f, 0.0f, zAtual}, 0.2f, 0.05f, tamSegmento, Fade(neon, alpha));
        DrawCube((Vector3){-1.0f, 0.0f, zAtual}, 0.1f, 0.05f, tamSegmento, Fade(neon, alpha * 0.5f));
        DrawCube((Vector3){ 1.0f, 0.0f, zAtual}, 0.1f, 0.05f, tamSegmento, Fade(neon, alpha * 0.5f));
        DrawCube((Vector3){ 3.0f, 0.0f, zAtual}, 0.2f, 0.05f, tamSegmento, Fade(neon, alpha));
    }
}

void descarregarCenario(Env *env)
{
    UnloadTexture(env->sphere.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture);
    UnloadModel(env->sphere);
}