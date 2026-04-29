#include "raylib.h"
#include "globals.h"
#include "menu.h"
#include "player.h"
#include "environment.h"
#include "rlgl.h"

GameState gameState = MENU;

int main(void) {
    InitWindow(800, 450, "AstroBeat");
    SetExitKey(0);
    SetTargetFPS(144);

    Camera3D camera = {0};
    camera.position = (Vector3){0.0f, 5.0f, 8.0f};
    camera.target = (Vector3){0.0f, 0.0f, -10.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Camera3D cameraBG = {0};
    cameraBG.position = (Vector3){0.0f, 0.0f, 0.0f};
    cameraBG.target = (Vector3){0.0f, -5.0f, -18.0f};
    cameraBG.up = (Vector3){0.0f, 1.0f, 0.0f};
    cameraBG.fovy = 120.0f;
    cameraBG.projection = CAMERA_PERSPECTIVE;

    Nave nave;
    inicializarNave(&nave);

    Env env;
    inicializarCenario(&env);

    while (true) {
            if (WindowShouldClose() || gameState == QUIT) {
                break;
            }

            float deltaTime = GetFrameTime();

            switch (gameState) {

                case MENU:
                    atualizarMenu(&gameState);
                    break;

                case SONG_SELECT:
                    atualizarSongSelect(&gameState);
                    break;

                case SETTINGS:
                    atualizarSettings(&gameState);
                    break;

                case PLAYING:
                    if (IsKeyPressed(KEY_ESCAPE)) {
                        gameState = PAUSED;
                    } else {
                        atualizarNave(&nave, deltaTime);
                        atualizarCenario(&env, deltaTime);
                    }
                    break;

                case PAUSED:
                    atualizarPause(&gameState);
                    break;

                case QUIT:
                    break;
            }

            if (gameState == QUIT) {
                break;
            }

            BeginDrawing();
                ClearBackground(BLACK);
                switch (gameState) {
                    case MENU:
                        desenharMenu();
                        break;

                    case SONG_SELECT:
                        desenharSongSelect();
                        break;

                    case SETTINGS:
                        desenharSettings();
                        break;

                    case PLAYING:
                        BeginMode3D(cameraBG);
                            rlDisableDepthMask();
                            desenharFundo(&env);
                            rlEnableDepthMask();
                        EndMode3D();

                        BeginMode3D(camera);
                            desenharPistaEstrelas(&env);
                            desenharNave(&nave);
                        EndMode3D();

                        DrawFPS(10, 10);
                        break;

                    case PAUSED:
                        BeginMode3D(cameraBG);
                            rlDisableDepthMask();
                            desenharFundo(&env);
                            rlEnableDepthMask();
                        EndMode3D();

                        BeginMode3D(camera);
                            desenharPistaEstrelas(&env);
                            desenharNave(&nave);
                        EndMode3D();

                        desenharPause();
                        break;
                }
            EndDrawing();
        }
        descarregarCenario(&env);
        descarregarNave(&nave);

        CloseWindow();
        return 0;
    }
