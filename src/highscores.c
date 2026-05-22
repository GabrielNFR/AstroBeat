#include "highscores.h"
#include "menu.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int *highscores = NULL;

void carregarHighscores(void)
{
    if (highscores == NULL)
    {
        highscores = (int*)malloc(TOTAL_SONGS * sizeof(int));
    }

    FILE *f = fopen(ARQUIVO_HIGHSCORES, "r");
    if (!f)
    {
        for (int i = 0; i < TOTAL_SONGS; i++)
        {
            highscores[i] = 0;
        }
        return;
    }

    for (int i = 0; i < TOTAL_SONGS; i++)
    {
        char nome[100];
        int pts;
        if (fscanf(f, "%99[^=]=%d\n", nome, &pts) == 2)
        {
            if (songList[i].nome && strcmp(nome, songList[i].nome) == 0)
            {
                highscores[i] = pts;
            }
            else
            {
                highscores[i] = 0;
            }
        }
        else
        {
            highscores[i] = 0;
        }
    }
    fclose(f);
}

void salvarHighscore(int songIndex, int pontos)
{
    if (songIndex < 0 || songIndex >= TOTAL_SONGS) return;
    if (highscores == NULL) return;
    if (pontos <= highscores[songIndex]) return;

    highscores[songIndex] = pontos;

    FILE *f = fopen(ARQUIVO_HIGHSCORES, "w");
    if (!f) return;

    for (int i = 0; i < TOTAL_SONGS; i++)
    {
        if (songList[i].nome)
        {
            fprintf(f, "%s=%d\n", songList[i].nome, highscores[i]);
        }
    }
    fclose(f);
}

int obterHighscore(int songIndex)
{
    if (songIndex < 0 || songIndex >= TOTAL_SONGS || highscores == NULL) return 0;
    return highscores[songIndex];
}

void desenharHighscores(void)
{
    if (highscores == NULL) return; 

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    DrawText("HIGH SCORES",
             screenWidth/2 - MeasureText("HIGH SCORES", 40)/2,
             screenHeight/6, 40, MAGENTA);

    DrawText("MUSICA", screenWidth/4 - 60, screenHeight/3, 20, RAYWHITE);
    DrawText("PONTOS", screenWidth*3/4 - 60, screenHeight/3, 20, RAYWHITE);

    DrawLine(screenWidth/4 - 80, screenHeight/3 + 25,
             screenWidth*3/4 + 40, screenHeight/3 + 25, Fade(MAGENTA, 0.5f));

    for (int i = 0; i < TOTAL_SONGS; i++) {
        int y = screenHeight/3 + 40 + i * 35;

        Color cor = (highscores[i] > 0) ? GOLD : GRAY;
        
        const char *nome = songList[i].nome ? songList[i].nome : "N/A";

        DrawText(nome, screenWidth/4 - 60, y, 20, cor);
        DrawText(TextFormat("%d", highscores[i]), screenWidth*3/4 - 60, y, 20, cor);
    }

    DrawText("ESC para voltar",
             screenWidth/2 - MeasureText("ESC para voltar", 16)/2,
             screenHeight - 40, 16, Fade(RAYWHITE, 0.6f));
}

void atualizarHighscores(GameState *state)
{
    if (IsKeyPressed(KEY_ESCAPE)) {
        *state = MENU;
    }
}