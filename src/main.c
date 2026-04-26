#include "raylib.h"
#include "globals.h"
#include "player.h"
#include "environment.h"

int main(void) {
    InitWindow(800, 450, "AstroBeat");
    SetTargetFPS(60);

    Camera3D camera = {0};
    camera.position = (Vector3){0.0f, 4.0f, 8.0f};
    camera.target = (Vector3){0.0f, 0.0f, -10.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 65.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Nave nave;
    inicializarNave(&nave);

    Env env;
    inicializarCenario(&env);

    while (!WindowShouldClose()) {

        float deltaTime = GetFrameTime();

        atualizarNave(&nave, deltaTime);
        atualizarCenario(&env, deltaTime);
        
        BeginDrawing();
            ClearBackground(BLACK); 

            BeginMode3D(camera);

                desenharPistaEstrelas(&env);
                desenharNave(&nave);

            EndMode3D();

            DrawFPS(10, 10);

        EndDrawing();
    }

    descarregarCenario(&env);
    descarregarNave(&nave);

    CloseWindow();
    return 0;
}