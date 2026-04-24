#include "raylib.h"

#define FAIXA_ESQUERDA -2.0f
#define FAIXA_CENTRO 0.0f
#define FAIXA_DIREITA 2.0f

int main(void) {
    InitWindow(800, 450, "AstroBeat");
    SetTargetFPS(60);

    Camera3D camera = {0};
    camera.position = (Vector3){0.0f, 3.0f, 8.0f};
    camera.target = (Vector3){0.0f, 0.0f, -10.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Vector3 posicaoNave = {FAIXA_CENTRO, 0.5f, 0.0f};

    while (!WindowShouldClose()) {
        
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
        {
            if (posicaoNave.x == FAIXA_CENTRO) posicaoNave.x = FAIXA_ESQUERDA;
            else if (posicaoNave.x == FAIXA_DIREITA) posicaoNave.x = FAIXA_CENTRO;
        }

        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
        {
            if (posicaoNave.x == FAIXA_CENTRO) posicaoNave.x = FAIXA_DIREITA;
            else if (posicaoNave.x == FAIXA_ESQUERDA) posicaoNave.x = FAIXA_CENTRO;
        }
        
        BeginDrawing();
            ClearBackground(BLACK); 

            BeginMode3D(camera);

                DrawGrid(20, 2.0f);

                DrawLine3D((Vector3){FAIXA_ESQUERDA, 0, 10}, (Vector3){FAIXA_ESQUERDA, 0, -50}, DARKGRAY);
                DrawLine3D((Vector3){FAIXA_CENTRO, 0, 10},    (Vector3){FAIXA_CENTRO, 0, -50},    DARKGRAY);
                DrawLine3D((Vector3){FAIXA_DIREITA, 0, 10},  (Vector3){FAIXA_DIREITA, 0, -50},  DARKGRAY);

                DrawCube(posicaoNave, 1.0f, 1.0f, 1.0f, RED);
                DrawCubeWires(posicaoNave, 1.0f, 1.0f, 1.0f, MAROON); 

            EndMode3D();

            DrawText("Aperte Esquerda/Direita para mover pelas faixas", 10, 10, 20, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}