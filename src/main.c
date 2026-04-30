#include "raylib.h"
#include "globals.h"
#include "menu.h"
#include "player.h"
#include "environment.h"
#include "notas.h"
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

    leitura_arquivo_musica();
    
    float tempo_inicio = GetTime();
    while (true) {
            if (WindowShouldClose() || gameState == QUIT) {
                break;
            }

            float deltaTime = GetFrameTime();
            //gettime será substituido por getmusictimeplayed quando houver musica
            float tempo_atual = GetTime() - tempo_inicio;

            switch (gameState) {

                case MENU:
                    atualizarMenu(&gameState);
                    atualizarFundoMenu(&env, deltaTime);
                    break;

                case SONG_SELECT:
                    atualizarSongSelect(&gameState);
                    atualizarFundoMenu(&env, deltaTime);
                    break;

                case SETTINGS:
                    atualizarSettings(&gameState);
                    atualizarFundoMenu(&env, deltaTime);
                    break;

                case PLAYING:
                    atualizarNave(&nave, deltaTime);
                    atualizarCenario(&env, deltaTime);
                    atualizar_notas(tempo_atual);

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
                        BeginMode3D(cameraBG);
                            rlDisableDepthMask();
                            desenharFundoMenu(&env);
                            rlEnableDepthMask();
                        EndMode3D();
                        desenharMenu();
                        break;


                    case SONG_SELECT:
                        BeginMode3D(cameraBG);
                            rlDisableDepthMask();
                            desenharFundoMenu(&env);
                            rlEnableDepthMask();
                        EndMode3D();
                        desenharSongSelect();
                        break;

                    case SETTINGS:
                        BeginMode3D(cameraBG);
                            rlDisableDepthMask();
                            desenharFundoMenu(&env);
                            rlEnableDepthMask();
                        EndMode3D();
                        desenharSettings();
                        break;

                    case PLAYING:
                        BeginMode3D(camera);
                            rlDisableDepthTest();
                            desenharFundo(&env);
                            rlEnableDepthTest();
                        EndMode3D();
                        BeginMode3D(camera);
                            desenharPistaEstrelas(&env);
                            desenharNave(&nave);
                            desenhar_notas(tempo_atual);
                        EndMode3D();
                        DrawFPS(10, 10);
                        break;
                }
            EndDrawing();
        }

        descarregarCenario(&env);
        descarregarNave(&nave);
    
        CloseWindow();
        return 0;
    }

    

