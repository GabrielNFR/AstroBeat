#include <stdio.h>
#include <math.h>
#include "menu.h"
#include "audio.h"
#include "highscores.h"
#include <raylib.h>

// variaveis
static int menuSelectedOption = 0;
static int pauseSelectedOption = 0;

// lista de musicas

const SongInfo songList[] = {
    {"Elektronomia", "songs/Elektronomia.ogg", "mapasMusicas/mapaElektronomia.txt", "mapasColetaveis/mapaElektronomia.txt"},
    {"Spektrem", "songs/Spektrem.mp3", "mapasMusicas/mapaSpektrem.txt", "mapasColetaveis/mapaSpektrem.txt"},
    {"DK Invincible", "songs/DKInvincible.mp3", "mapasMusicas/mapaDKInvincible.txt", "mapasColetaveis/mapaDKInvincible.txt"},
    {"Dancefloor Dreamer", "songs/Dancefloor Dreamer.mp3", "mapasMusicas/mapaDancefloorDreamer.txt", NULL},
    {"Teste", NULL, NULL, NULL}
};

int songSelectOption = 0;

static const Color UI_INK = {238, 240, 255, 255};
static const Color UI_MUTED = {154, 160, 190, 255};
static const Color UI_DIM = {92, 96, 124, 255};
static const Color UI_PANEL = {9, 11, 28, 218};
static const Color UI_PANEL_SOFT = {18, 20, 46, 190};
static const Color UI_PANEL_HOT = {52, 21, 78, 230};
static const Color UI_ACCENT = {222, 67, 255, 255};
static const Color UI_CYAN = {55, 216, 255, 255};
static const Color UI_GOLD = {255, 202, 82, 255};
static const Color UI_DANGER = {255, 84, 112, 255};

static void drawOverlay(float alpha)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade((Color){3, 4, 14, 255}, alpha));
}

static void drawTextShadow(const char *text, int x, int y, int fontSize, Color color)
{
    DrawText(text, x + 2, y + 2, fontSize, Fade(BLACK, 0.55f));
    DrawText(text, x, y, fontSize, color);
}

static void drawTextCentered(const char *text, int centerX, int y, int fontSize, Color color)
{
    drawTextShadow(text, centerX - MeasureText(text, fontSize) / 2, y, fontSize, color);
}

static void drawPanel(Rectangle rec, Color fill, Color border)
{
    DrawRectangleRounded(rec, 0.08f, 12, fill);
    DrawRectangleRoundedLinesEx(rec, 0.08f, 12, 1.5f, border);
}

static void drawHintPill(Rectangle rec, const char *text, Color color)
{
    DrawRectangleRounded(rec, 0.45f, 12, Fade(color, 0.16f));
    DrawRectangleRoundedLinesEx(rec, 0.45f, 12, 1.0f, Fade(color, 0.55f));
    DrawText(text, (int)(rec.x + rec.width / 2 - MeasureText(text, 14) / 2), (int)(rec.y + rec.height / 2 - 7), 14, color);
}

static void drawMenuButton(Rectangle rec, const char *label, const char *hint, bool selected, bool enabled)
{
    Color fill = enabled ? UI_PANEL_SOFT : (Color){18, 19, 30, 150};
    Color border = enabled ? Fade(UI_MUTED, 0.35f) : Fade(UI_DIM, 0.25f);
    Color labelColor = enabled ? UI_INK : UI_DIM;
    Color hintColor = enabled ? UI_MUTED : Fade(UI_DIM, 0.75f);

    if (selected && enabled)
    {
        fill = UI_PANEL_HOT;
        border = Fade(UI_ACCENT, 0.95f);
    }
    else if (selected)
    {
        border = Fade(UI_DIM, 0.65f);
    }

    drawPanel(rec, fill, border);

    if (selected)
    {
        float pulse = 0.45f + 0.20f * sinf((float)GetTime() * 5.0f);
        DrawCircle((int)(rec.x + 24), (int)(rec.y + rec.height / 2), 5.0f, Fade(enabled ? UI_ACCENT : UI_DIM, pulse));
    }

    drawTextShadow(label, (int)(rec.x + 44), (int)(rec.y + 13), 22, labelColor);
    if (hint && hint[0] != '\0')
    {
        DrawText(hint, (int)(rec.x + 44), (int)(rec.y + 42), 14, hintColor);
    }
}

static void drawFooter(const char *text)
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    DrawText(text,
             screenWidth / 2 - MeasureText(text, 16) / 2,
             screenHeight - 34,
             16,
             Fade(UI_MUTED, 0.85f));
}

static bool musicaDisponivel(int index)
{
    if (index < 0 || index >= TOTAL_SONGS) return false;
    return songList[index].caminhoMusica != NULL && songList[index].caminhoBeatmap != NULL;
}

static const char *statusMusica(int index)
{
    if (musicaDisponivel(index)) return "PRONTA";
    if (songList[index].caminhoMusica == NULL) return "SEM AUDIO";
    if (songList[index].caminhoBeatmap == NULL) return "SEM MAPA";
    return "INDISPONIVEL";
}

static void moverSelecaoCircular(int *opcao, int total, int direcao)
{
    *opcao += direcao;
    if (*opcao < 0) *opcao = total - 1;
    if (*opcao >= total) *opcao = 0;
}

const SongInfo* obterMusicaSelecionada(void)
{
    return &songList[songSelectOption];
}

// main menu
void atualizarMenu(GameState *state) {
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        moverSelecaoCircular(&menuSelectedOption, 4, -1);
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        moverSelecaoCircular(&menuSelectedOption, 4, 1);
    }
    if (IsKeyPressed(KEY_ENTER)) {
        if (menuSelectedOption == 0) {
            *state = SONG_SELECT;
            songSelectOption = 0;
        } else if (menuSelectedOption == 1){
            *state = SETTINGS;
        } else if (menuSelectedOption == 2){
            *state = HIGHSCORES;
        } else if (menuSelectedOption == 3){
            *state = QUIT;
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        *state = QUIT;
    }
}

void desenharMenu(void) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    float t = (float)GetTime();

    drawOverlay(0.42f);

    DrawText("RHYTHM RUNNER // DEEP SPACE",
             screenWidth / 2 - MeasureText("RHYTHM RUNNER // DEEP SPACE", 14) / 2,
             screenHeight / 6 - 26,
             14,
             Fade(UI_CYAN, 0.8f));

    drawTextCentered("AstroBeat", screenWidth / 2, screenHeight / 6, 64, UI_INK);
    DrawLine(screenWidth / 2 - 150, screenHeight / 6 + 78,
             screenWidth / 2 + 150, screenHeight / 6 + 78,
             Fade(UI_ACCENT, 0.55f + 0.20f * sinf(t * 2.0f)));

    DrawText("Pilote no pulso da musica. Acerte notas, colete buffs e sobreviva ao fluxo.",
             screenWidth / 2 - MeasureText("Pilote no pulso da musica. Acerte notas, colete buffs e sobreviva ao fluxo.", 18) / 2,
             screenHeight / 6 + 96,
             18,
             UI_MUTED);

    float buttonWidth = 430.0f;
    float buttonHeight = 68.0f;
    float startX = screenWidth / 2 - buttonWidth / 2;
    float startY = screenHeight / 2 - 70.0f;
    float gap = 14.0f;

    drawMenuButton((Rectangle){startX, startY + 0 * (buttonHeight + gap), buttonWidth, buttonHeight},
                   "Lista de Musicas", "Escolha uma faixa e inicie a corrida", menuSelectedOption == 0, true);
    drawMenuButton((Rectangle){startX, startY + 1 * (buttonHeight + gap), buttonWidth, buttonHeight},
                   "Configuracoes", "Volume e ajustes de experiencia", menuSelectedOption == 1, true);
    drawMenuButton((Rectangle){startX, startY + 2 * (buttonHeight + gap), buttonWidth, buttonHeight},
                   "High Scores", "Veja seus melhores resultados", menuSelectedOption == 2, true);
    drawMenuButton((Rectangle){startX, startY + 3 * (buttonHeight + gap), buttonWidth, buttonHeight},
                   "Sair", "Fechar AstroBeat", menuSelectedOption == 3, true);

    Rectangle helpPanel = {screenWidth - 330.0f, screenHeight - 190.0f, 250.0f, 112.0f};
    drawPanel(helpPanel, Fade(UI_PANEL, 0.82f), Fade(UI_CYAN, 0.25f));
    DrawText("CONTROLES", (int)helpPanel.x + 20, (int)helpPanel.y + 16, 15, UI_CYAN);
    DrawText("A/D ou setas: mover", (int)helpPanel.x + 20, (int)helpPanel.y + 42, 14, UI_MUTED);
    DrawText("J/K: notas  |  SPACE: longa", (int)helpPanel.x + 20, (int)helpPanel.y + 64, 14, UI_MUTED);
    DrawText("ESC: pausar ou voltar", (int)helpPanel.x + 20, (int)helpPanel.y + 86, 14, UI_MUTED);

    drawFooter("W/S ou setas para navegar | ENTER para selecionar | ESC para sair");
}

// song select
void atualizarSongSelect(GameState *state) {
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        moverSelecaoCircular(&songSelectOption, TOTAL_SONGS, -1);
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        moverSelecaoCircular(&songSelectOption, TOTAL_SONGS, 1);
    }
    if (IsKeyPressed(KEY_ENTER) && musicaDisponivel(songSelectOption)) *state = PLAYING;
    if (IsKeyPressed(KEY_ESCAPE)) *state = MENU;
}

void desenharSongSelect(void) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    drawOverlay(0.48f);

    DrawText("SELECAO DE FAIXA", 86, 54, 14, Fade(UI_CYAN, 0.85f));
    drawTextShadow("Lista de Musicas", 84, 78, 42, UI_INK);
    DrawText("Escolha uma musica pronta para iniciar. Faixas sem audio ou mapa ficam bloqueadas.",
             86, 128, 17, UI_MUTED);

    Rectangle listPanel = {80.0f, 170.0f, 470.0f, screenHeight - 250.0f};
    Rectangle detailPanel = {590.0f, 170.0f, screenWidth - 670.0f, screenHeight - 250.0f};

    drawPanel(listPanel, UI_PANEL, Fade(UI_ACCENT, 0.28f));
    drawPanel(detailPanel, UI_PANEL, Fade(UI_CYAN, 0.28f));

    for (int i = 0; i < TOTAL_SONGS; i++) {
        bool selected = (i == songSelectOption);
        bool enabled = musicaDisponivel(i);
        float y = listPanel.y + 24.0f + i * 66.0f;
        Rectangle row = {listPanel.x + 18.0f, y, listPanel.width - 36.0f, 54.0f};

        Color rowFill = selected ? (enabled ? UI_PANEL_HOT : (Color){32, 32, 45, 210}) : Fade(UI_PANEL_SOFT, 0.65f);
        Color rowBorder = selected ? (enabled ? Fade(UI_ACCENT, 0.95f) : Fade(UI_DIM, 0.7f)) : Fade(UI_MUTED, 0.18f);
        Color nameColor = enabled ? UI_INK : UI_DIM;
        Color metaColor = enabled ? UI_MUTED : Fade(UI_DIM, 0.75f);

        drawPanel(row, rowFill, rowBorder);
        DrawText(TextFormat("%02d", i + 1), (int)row.x + 14, (int)row.y + 17, 16, selected ? UI_CYAN : UI_DIM);
        drawTextShadow(songList[i].nome, (int)row.x + 54, (int)row.y + 9, 20, nameColor);
        DrawText(statusMusica(i), (int)row.x + 56, (int)row.y + 33, 12, metaColor);

        if (enabled)
        {
            const char *scoreText = TextFormat("%d", obterHighscore(i));
            DrawText(scoreText,
                     (int)(row.x + row.width - MeasureText(scoreText, 14) - 16),
                     (int)row.y + 20,
                     14,
                     UI_GOLD);
        }
    }

    const SongInfo *song = obterMusicaSelecionada();
    bool enabled = musicaDisponivel(songSelectOption);

    DrawText("FAIXA SELECIONADA", (int)detailPanel.x + 28, (int)detailPanel.y + 28, 14, UI_CYAN);
    drawTextShadow(song->nome, (int)detailPanel.x + 28, (int)detailPanel.y + 58, 34, enabled ? UI_INK : UI_DIM);

    drawHintPill((Rectangle){detailPanel.x + 28, detailPanel.y + 112, 120, 28}, statusMusica(songSelectOption), enabled ? UI_CYAN : UI_DANGER);

    DrawText("High score", (int)detailPanel.x + 28, (int)detailPanel.y + 166, 16, UI_MUTED);
    DrawText(TextFormat("%d", obterHighscore(songSelectOption)), (int)detailPanel.x + 28, (int)detailPanel.y + 190, 38, enabled ? UI_GOLD : UI_DIM);

    DrawText("Conteudo", (int)detailPanel.x + 28, (int)detailPanel.y + 254, 16, UI_MUTED);
    DrawText(song->caminhoMusica ? "Audio encontrado" : "Audio ausente",
             (int)detailPanel.x + 28, (int)detailPanel.y + 282, 18, song->caminhoMusica ? UI_INK : UI_DANGER);
    DrawText(song->caminhoBeatmap ? "Mapa encontrado" : "Mapa ausente",
             (int)detailPanel.x + 28, (int)detailPanel.y + 310, 18, song->caminhoBeatmap ? UI_INK : UI_DANGER);
    DrawText(song->caminhoColetaveis ? "Coletaveis ativos" : "Sem coletaveis extras",
             (int)detailPanel.x + 28, (int)detailPanel.y + 338, 18, song->caminhoColetaveis ? UI_INK : UI_MUTED);

    Rectangle cta = {detailPanel.x + 28, detailPanel.y + detailPanel.height - 86, detailPanel.width - 56, 56};
    drawMenuButton(cta,
                   enabled ? "ENTER para iniciar" : "Faixa bloqueada",
                   enabled ? "Prepare-se para a contagem e mantenha o ritmo" : "Adicione audio e mapa para liberar esta faixa",
                   true,
                   enabled);

    drawFooter("W/S ou setas para navegar | ENTER para iniciar | ESC para voltar");
}

// settings
void atualizarSettings(GameState *state, Audio *audio) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        *state = MENU;
    }

    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
        definirVolumeMusica(audio, audio->volumeMusica - 0.05f);
    }

    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
        definirVolumeMusica(audio, audio->volumeMusica + 0.05f);
    }
}

void desenharSettings(Audio *audio) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    drawOverlay(0.52f);

    Rectangle panel = {screenWidth / 2.0f - 330.0f, screenHeight / 2.0f - 210.0f, 660.0f, 390.0f};
    drawPanel(panel, UI_PANEL, Fade(UI_ACCENT, 0.35f));

    DrawText("SISTEMA", (int)panel.x + 34, (int)panel.y + 30, 14, UI_CYAN);
    drawTextShadow("Configuracoes", (int)panel.x + 32, (int)panel.y + 54, 38, UI_INK);
    DrawText("Ajuste o conforto antes da corrida.", (int)panel.x + 34, (int)panel.y + 102, 17, UI_MUTED);

    float volume = audio->volumeMusica;
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;

    DrawText("Volume da musica", (int)panel.x + 34, (int)panel.y + 154, 18, UI_INK);
    DrawText(TextFormat("%.0f%%", volume * 100.0f), (int)(panel.x + panel.width - 104), (int)panel.y + 154, 18, UI_CYAN);

    Rectangle barBack = {panel.x + 34, panel.y + 190, panel.width - 68, 22};
    DrawRectangleRounded(barBack, 0.45f, 12, Fade(UI_PANEL_SOFT, 0.9f));
    DrawRectangleRounded((Rectangle){barBack.x, barBack.y, barBack.width * volume, barBack.height}, 0.45f, 12, UI_ACCENT);
    DrawRectangleRoundedLinesEx(barBack, 0.45f, 12, 1.0f, Fade(UI_MUTED, 0.35f));

    DrawText("Em breve", (int)panel.x + 34, (int)panel.y + 252, 16, UI_MUTED);
    DrawText("Calibragem de atraso, intensidade visual e remapeamento de teclas.",
             (int)panel.x + 34, (int)panel.y + 278, 17, Fade(UI_MUTED, 0.9f));

    drawFooter("A/D ou setas esquerda/direita para ajustar | ESC para voltar");
}

// pause
void atualizarPause(GameState *state) {
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        moverSelecaoCircular(&pauseSelectedOption, 3, -1);
    }

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        moverSelecaoCircular(&pauseSelectedOption, 3, 1);
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        *state = PLAYING;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        if (pauseSelectedOption == 0) {
            *state = PLAYING;
        } else if (pauseSelectedOption == 1) {
            *state = MENU;
        } else if (pauseSelectedOption == 2) {
            *state = QUIT;
        }
    }

    if (IsKeyPressed(KEY_Q)) {
        *state = QUIT;
    }
}

void desenharPause(void) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    drawOverlay(0.66f);

    Rectangle panel = {screenWidth / 2.0f - 260.0f, screenHeight / 2.0f - 220.0f, 520.0f, 420.0f};
    drawPanel(panel, UI_PANEL, Fade(UI_ACCENT, 0.45f));

    DrawText("CORRIDA INTERROMPIDA", (int)panel.x + 36, (int)panel.y + 32, 14, UI_CYAN);
    drawTextShadow("Pausado", (int)panel.x + 34, (int)panel.y + 58, 46, UI_INK);
    DrawText("Respire, ajuste o ritmo e volte quando estiver pronto.",
             (int)panel.x + 36, (int)panel.y + 112, 16, UI_MUTED);

    float buttonWidth = panel.width - 72.0f;
    float buttonHeight = 62.0f;
    float startX = panel.x + 36.0f;
    float startY = panel.y + 162.0f;
    float gap = 14.0f;

    drawMenuButton((Rectangle){startX, startY + 0 * (buttonHeight + gap), buttonWidth, buttonHeight},
                   "Continuar", "Voltar para a musica", pauseSelectedOption == 0, true);
    drawMenuButton((Rectangle){startX, startY + 1 * (buttonHeight + gap), buttonWidth, buttonHeight},
                   "Voltar ao menu", "Encerrar esta tentativa", pauseSelectedOption == 1, true);
    drawMenuButton((Rectangle){startX, startY + 2 * (buttonHeight + gap), buttonWidth, buttonHeight},
                   "Sair do jogo", "Fechar AstroBeat", pauseSelectedOption == 2, true);

    drawFooter("W/S ou setas para navegar | ENTER para selecionar | ESC para continuar | Q para sair");
}
