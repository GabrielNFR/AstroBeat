#include "raylib.h"

#define FAIXA_ESQUERDA -2.0f
#define FAIXA_CENTRO 0.0f
#define FAIXA_DIREITA 2.0f
#define QTD_ESTRELAS 70

int main(void) {
    InitWindow(800, 450, "AstroBeat - Velocidade para Frente!");

    Texture2D nebulaTexture = LoadTexture("assets/nebulabg1.png");

    SetTargetFPS(60);

    Camera3D camera = {0};
    camera.position = (Vector3){0.0f, 3.0f, 8.0f};
    camera.target = (Vector3){0.0f, 0.0f, -10.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Vector3 posicaoNave = {FAIXA_CENTRO, 0.5f, 0.0f};

    float velocidadeVoo = 40.0f; 
    Vector3 estrelas[QTD_ESTRELAS];

    for (int i = 0; i < QTD_ESTRELAS; i++) {
        do {
            estrelas[i].x = (float)GetRandomValue(-20, 20);
            estrelas[i].y = (float)GetRandomValue(-5, 15);
        } while (estrelas[i].x > -4.0f && estrelas[i].x < 4.0f && estrelas[i].y > -1.0f && estrelas[i].y < 5.0f);

        estrelas[i].z = (float)GetRandomValue(-50, 10);
    }

    while (!WindowShouldClose()) {

        float deltaTime = GetFrameTime();

        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
            if (posicaoNave.x == FAIXA_CENTRO) posicaoNave.x = FAIXA_ESQUERDA;
            else if (posicaoNave.x == FAIXA_DIREITA) posicaoNave.x = FAIXA_CENTRO;
        }

        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
            if (posicaoNave.x == FAIXA_CENTRO) posicaoNave.x = FAIXA_DIREITA;
            else if (posicaoNave.x == FAIXA_ESQUERDA) posicaoNave.x = FAIXA_CENTRO;
        }

        for (int i = 0; i < QTD_ESTRELAS; i++) {
            estrelas[i].z += velocidadeVoo * deltaTime;

            // Se a estrela passou da câmera (Z > 10), ela "renasce" lá no fundo (Z = -50)
            if (estrelas[i].z > 10.0f) {
                estrelas[i].z = -50.0f;
                // Sorteia nova posição horizontal e vertical para não ficar padronizado
                do {
                estrelas[i].x = (float)GetRandomValue(-20, 20);
                estrelas[i].y = (float)GetRandomValue(-5, 15);
                } while (estrelas[i].x > -4.0f && estrelas[i].x < 4.0f && estrelas[i].y > -1.0f && estrelas[i].y < 5.0f);
            }
        }
        
        BeginDrawing();
            ClearBackground(BLACK); 

            Rectangle sourceRec = {0.0f, 0.0f, (float)nebulaTexture.width, (float)nebulaTexture.height};
            Rectangle destRec = {0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()};
            Vector2 origin = {0.0f, 0.0f};

            DrawTexturePro(nebulaTexture, sourceRec, destRec, origin, 0.0f, WHITE);

            BeginMode3D(camera);

                for (int i = 0; i < QTD_ESTRELAS; i++) {
                    DrawCube(estrelas[i], 0.1f, 0.1f, 1.0f, RAYWHITE); 
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

                DrawCube(posicaoNave, 1.0f, 1.0f, 1.0f, RED);
                DrawCubeWires(posicaoNave, 1.0f, 1.0f, 1.0f, MAROON); 

            EndMode3D();

            DrawFPS(10, 10);

        EndDrawing();
    }

    UnloadTexture(nebulaTexture);
    CloseWindow();
    return 0;
}