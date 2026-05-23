#include "raylib.h"
#include "globals.h"
#include "menu.h"
#include "buffs.h"
#include "player.h"
#include "environment.h"
#include "notas.h"
#include "notas_modelos.h"
#include "score.h"
#include "rlgl.h"
#include "audio.h"
#include "highscores.h"
#include <stdio.h>

GameState gameState = MENU;

int main(void) {
    int windowedWidth = 1280;
    int windowedHeight = 720;
    bool windowSizeSaved = false;
    
    InitWindow(windowedWidth, windowedHeight, "AstroBeat");
    carregarModelosNotas();
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

    Audio audio;
    audio.musicaCarregada = false;
    audio.volumeMusica = carregarVolumeConfigurado();

    Score score;
    init_sistema_pontos(&score);
    
    float tempo_jogo = 0.0f;
    float tempo_inicio_musica = 0.0f;

    carregarHighscores();

    while (true) {
            if (WindowShouldClose() || gameState == QUIT) {
                break;
            }

            float deltaTime = GetFrameTime();
            GameState estadoAnterior = gameState;

            atualizarAudio(&audio);

            if (gameState == PLAYING || gameState == PAUSED) {
                tempo_jogo = obterTempoMusica(&audio) - tempo_inicio_musica;
                if (tempo_jogo < 0.0f) {
                    tempo_jogo = 0.0f;
                }
            }

            if (IsKeyPressed(KEY_F11)) {
                if (!IsWindowState(FLAG_WINDOW_UNDECORATED)) {
                    windowedWidth = GetScreenWidth();
                    windowedHeight = GetScreenHeight();
                    windowSizeSaved = true;

                    int monitor = GetCurrentMonitor();
                    Vector2 pos = GetMonitorPosition(monitor);
                    SetWindowPosition((int)pos.x, (int)pos.y);
                    SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
                    SetWindowState(FLAG_WINDOW_UNDECORATED);
                } else {
                    ClearWindowState(FLAG_WINDOW_UNDECORATED);
                    SetWindowSize(windowedWidth, windowedHeight);
                    // Centraliza a janela após restaurar
                    int monitor = GetCurrentMonitor();
                    Vector2 pos = GetMonitorPosition(monitor);
                    SetWindowPosition(
                        (int)pos.x + (GetMonitorWidth(monitor) - windowedWidth) / 2,
                        (int)pos.y + (GetMonitorHeight(monitor) - windowedHeight) / 2
                    );
                }
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
                    atualizarSettings(&gameState, &audio);
                    atualizarFundoMenu(&env, deltaTime);
                    break;

                case PLAYING:
                     if (IsKeyPressed(KEY_ESCAPE)) {
                        gameState = PAUSED;
                    } else {
                        // DEBUG: skip na música
                        if (IsKeyPressed(KEY_F)) {
                            float novoTempo = tempo_jogo + 10.0f;
                            seekMusica(&audio, novoTempo);
                            tempo_inicio_musica = obterTempoMusica(&audio) - novoTempo;
                            tempo_jogo = novoTempo;
                            printf("⏩ Skip +10s → %.1fs\n", tempo_jogo);
                            fflush(stdout);
                        }
                        if (nave.buffMultiplicador && tempo_jogo >= nave.tempoFimMult) {
                            nave.buffMultiplicador = false;
                            printf("BUFF MULTIPLICADOR EXPIRADO\n");
                            fflush(stdout);
                        }
                        if (nave.buffJanela && tempo_jogo >= nave.tempoFimJanela) {
                            nave.buffJanela = false;
                            printf("BUFF JANELA EXPIRADO\n");
                            fflush(stdout);
                        }
                        atualizarNave(&nave, deltaTime);
                        atualizarCenario(&env, deltaTime, tempo_jogo, songSelectOption);
                        camera.fovy += (env.fovAlvo - camera.fovy) * deltaTime * 3.0f;
                        atualizar_notas(tempo_jogo);
                        verificarAcertos(&nave, &score, tempo_jogo, deltaTime);
                        atualizarColetavel(tempo_jogo);
                        verificarColisao(&nave, tempo_jogo);
                        
                        if (musicaTerminou(&audio)){
                            calcularPrecisao(&score);
                            salvarHighscore(songSelectOption, score.pontos);
                            gameState = RESULTS;
                        }
                    }
                    break;

                case PAUSED:
                    camera.fovy += (env.fovAlvo - camera.fovy) * deltaTime * 3.0f;
                    atualizarPause(&gameState);
                    break;

                case RESULTS:
                    atualizarResultados(&gameState);
                    break;
                
                case HIGHSCORES:
                    atualizarHighscores(&gameState);
                    atualizarFundoMenu(&env, deltaTime);
                    break;

                case QUIT:
                    break;
            }

            if (estadoAnterior != gameState) {
                if (estadoAnterior == SONG_SELECT && gameState == PLAYING) {
                    const SongInfo *song = obterMusicaSelecionada();
                    
                    float volumeSalvo = audio.volumeMusica;

                    init_sistema_pontos(&score);
                    resetar_notas();
                    limparColetavel();
                    leitura_arquivo_musica(song->caminhoBeatmap);
                    calcularTotais(&score);
                    leitura_arquivo_coletaveis(song->caminhoColetaveis);
                    
                    if (audio.musicaCarregada)
                    {
                        pararMusica(&audio);
                        descarregarAudio(&audio);
                    }

                    if (song->caminhoMusica)
                    {
                        inicializarAudio(&audio, song->caminhoMusica);
                        definirVolumeMusica(&audio, volumeSalvo);
                    }
                    else
                    {
                        audio.musicaCarregada = false;
                        audio.volumeMusica = volumeSalvo;
                    }

                    iniciarMusica(&audio);
                    tempo_inicio_musica = obterTempoMusica(&audio);
                    tempo_jogo = 0.0f;
                }

                if (estadoAnterior == PLAYING && gameState == PAUSED) {
                    pausarMusica(&audio);
                    printf("PAUSADO em: %.4f\n", tempo_jogo);
                }

                if (estadoAnterior == PAUSED && gameState == PLAYING) {
                    continuarMusica(&audio);
                }

                if ((estadoAnterior == PLAYING || estadoAnterior == PAUSED) &&
                    (gameState == MENU || gameState == QUIT)) {
                    pararMusica(&audio);
                    resetar_notas();
                    limparColetavel();
                    tempo_inicio_musica = 0.0f;
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
                        desenharSettings(&audio);
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
                            desenharColetavel(tempo_jogo);
                        EndMode3D();
                        draw_sistema_pontos(&score);
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
                            desenharColetavel(tempo_jogo);
                        EndMode3D();

                        desenharPause();
                        DrawFPS(10, 10);
                        break;

                    case RESULTS:
                        ClearBackground(BLACK);

                        draw_Resultados(&score);

                        break;

                    case HIGHSCORES:
                        BeginMode3D(cameraBG);
                            rlDisableDepthMask();
                            desenharFundoMenu(&env);
                            rlEnableDepthMask();
                        EndMode3D();
                        desenharHighscores();
                        break;
                    
                    case QUIT:
                        break;
                }
            EndDrawing();
        }

        limparColetavel();
        descarregarModelosNotas();
        descarregarCenario(&env);
        descarregarNave(&nave);
        descarregarAudio(&audio);

        CloseWindow();
        return 0;
    }
