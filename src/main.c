#include "raylib.h"
#include "globals.h"
#include "menu.h"
#include "player.h"
#include "environment.h"
#include "notas.h"
#include "rlgl.h"
#include "audio.h"
#include <stdio.h>

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
    Audio audio;
    inicializarAudio(&audio, "songs/Elektronomia.ogg");

    float tempo_jogo = 0.0f;
    while (true) {
            if (WindowShouldClose() || gameState == QUIT) {
                break;
            }

            float deltaTime = GetFrameTime();
            GameState estadoAnterior = gameState;

            atualizarAudio(&audio);

            if (gameState == PLAYING || gameState == PAUSED) {
                tempo_jogo = obterTempoMusica(&audio);
            }

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
                     if (IsKeyPressed(KEY_ESCAPE)) {
                        gameState = PAUSED;
                    } else {
                        // DEBUG: skip na música
                        if (IsKeyPressed(KEY_F)) {
                            float t = obterTempoMusica(&audio);
                            seekMusica(&audio, t + 10.0f);
                            printf("⏩ Skip +10s → %.1fs\n", obterTempoMusica(&audio));
                            fflush(stdout);
                        }
                        atualizarNave(&nave, deltaTime);
                        atualizarCenario(&env, deltaTime);
                        atualizar_notas(tempo_jogo);
                        verificarAcertos(&nave, tempo_jogo, deltaTime);
                    }
                    break;

                case PAUSED:
                    atualizarPause(&gameState);
                    break;

                case QUIT:
                    break;
            }

            if (estadoAnterior != gameState) {
                if (estadoAnterior == SONG_SELECT && gameState == PLAYING) {
                    resetar_notas();
                    iniciarMusica(&audio);
                    tempo_jogo = 0.0f;
                }

                if (estadoAnterior == PLAYING && gameState == PAUSED) {
                    pausarMusica(&audio);
                }

                if (estadoAnterior == PAUSED && gameState == PLAYING) {
                    continuarMusica(&audio);
                }

                if ((estadoAnterior == PLAYING || estadoAnterior == PAUSED) &&
                    (gameState == MENU || gameState == QUIT)) {
                    pararMusica(&audio);
                    tempo_jogo = 0.0f;
                }
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
                        BeginMode3D(cameraBG);
                            rlDisableDepthTest();
                            desenharFundo(&env);
                            rlEnableDepthTest();
                        EndMode3D();
                        BeginMode3D(camera);
                            desenharPistaEstrelas(&env);
                            desenharNave(&nave);
                            desenhar_notas(tempo_jogo);
                        EndMode3D();
                        DrawFPS(10, 10);
                        break;

                     case PAUSED:
                        BeginMode3D(cameraBG);
                            rlDisableDepthTest();
                            desenharFundo(&env);
                            rlEnableDepthTest();
                        EndMode3D();
                        BeginMode3D(camera);
                            desenharPistaEstrelas(&env);
                            desenharNave(&nave);
                            desenhar_notas(tempo_jogo);
                        EndMode3D();

                        desenharPause();
                        DrawFPS(10, 10);
                        break;
                }
            EndDrawing();
        }

        descarregarCenario(&env);
        descarregarNave(&nave);
        descarregarAudio(&audio);

        CloseWindow();
        return 0;
    }
