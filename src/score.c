#include "raylib.h"
#include "globals.h"
#include "notas.h"
#include "score.h"
#include "buffs.h"
#include <stdio.h>
#include <math.h>

const char *nomesLinhas[TOTAL_LINHAS] = {
    "GRAVE",
    "AGUDO",
    "LONGA",
    "MOVE",
    "GERAL"
};

const char *nomesColunas[TOTAL_COLUNAS] = {"TOTAL","ACERTOS","MISS","PRECISAO"};

void init_sistema_pontos(Score *score){
    score->pontos = 0;
    score->streak = 0;
    score->maior_streak = 0;
    score->multiplicador = 1.0f;
    score->ultima_nota = JULG_PENDENTE;
    for(int lin = 0; lin < TOTAL_LINHAS; lin++){
        for(int col = 0; col < TOTAL_COLUNAS; col++){
            score->matrizResultados[lin][col] = 0;
            }
        }
}

float multiplicador_julgamento(Julgamento nota){
    switch (nota){
    case JULG_OK:         return 1.0f;
    case JULG_GOOD:       return 1.25f;
    case JULG_GREAT:      return 1.5f;
    case JULG_PERFECT:    return 2.0f;

    default:              return 0.0f;
    }
}

float multiplicador_streak(int streak){
    if (streak>=30) return 8.0f;
    if (streak>=20) return 4.0f;
    if (streak>=10) return 2.0f;

    return 1.0f;
}
void aumentar_streak(Score *score,int valor){
    score->streak+=valor;
    
    if (score->streak > score->maior_streak){
        score->maior_streak=score->streak;
    }
    
    score->multiplicador= multiplicador_streak(score->streak);
    
}

void adicionar_pontos(Score *score,Julgamento nota,int pontosBase){
    float multjulg= multiplicador_julgamento(nota);

    int pontosganhos= (pontosBase * multjulg * score->multiplicador);
    score->pontos += pontosganhos;
    score->ultima_nota = nota;

}

void errar_nota(Score * score){
    score->streak = 0;
    score->multiplicador= 1.0f;
    score->ultima_nota = JULG_MISS;
}

const char* HitToString(Julgamento nota)
{
    switch(nota)
    {
        case JULG_PERFECT:
            return "PERFECT";

        case JULG_GREAT:
            return "GREAT";

        case JULG_GOOD:
            return "GOOD";

        case JULG_OK:
            return "OK";

        case JULG_MISS:
            return "MISS";

        case JULG_PENDENTE:
            return "";
        default:
            return "";
    }
}

static void desenharTextoJulgamento(const char *texto, Julgamento nota)
{
    int fonte = 40;
    int largura = MeasureText(texto, fonte);
    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();
    int x = screenW - largura - 220;
    int y = screenH - 340;

    Color cor;
    switch (nota) {
        case JULG_PERFECT: cor = GOLD;    break;
        case JULG_GREAT:   cor = PURPLE;  break;
        case JULG_GOOD:    cor = BLUE;    break;
        case JULG_OK:      cor = GREEN;   break;
        case JULG_MISS:    cor = WHITE;   break;
        default:           cor = WHITE;   break;
    }

    DrawText(texto, x, y, fonte, cor);
}

void draw_sistema_pontos(Score*score){
    const char *texto = HitToString(score->ultima_nota);
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    float pulse =sinf(GetTime() * 8.0f) * 5.0f;
    int tamanhoMult = 80 + pulse;

    Color corMultiplicador = GREEN;
    Color multColor = (Color){0, 255, 255, 255};
    Color multColor2 = (Color){255, 0, 200, 255};
    Color fogo ={255,100 + (int)(sinf(GetTime() * 10) * 80),0,255};

    if(score->multiplicador==2){
        corMultiplicador = multColor;
    }
    else if(score->multiplicador==4){
        corMultiplicador = BLUE;
    }
     if(score->multiplicador==8){
        tamanhoMult = 40 + sinf(GetTime() * 8.0f) * 5.0f;
        corMultiplicador =(Color){255,140 + (int)(sinf(GetTime() * 12) * 60),0,255};
    }

    int fonte = 40;

    int larguraTexto =MeasureText(texto, fonte);

    DrawText(TextFormat("Pontuação: %d", score->pontos),20, 40, 40, multColor2);
    DrawText(TextFormat("%d", score->streak),140, screenHeight -135, 40, YELLOW);
    DrawText(TextFormat("%.0fx", score->multiplicador),140, screenHeight - 90,  tamanhoMult, corMultiplicador);
    desenharTextoJulgamento(HitToString(score->ultima_nota), score->ultima_nota);
}

void draw_Resultados(Score *score){
    
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    const char *titulo =
        "FIM DE MUSICA";

    int tituloSize = 40;

    int larguraTitulo =MeasureText(titulo, tituloSize);

    DrawText(titulo,(screenWidth - larguraTitulo)/2,40,tituloSize,WHITE);
    
    int cellWidth = 110;
    int cellHeight = 45;
    int larguraTabela = (TOTAL_COLUNAS + 1) * cellWidth;
    int alturaTabela = (TOTAL_LINHAS + 1) * cellHeight;
    int startX =  (screenWidth - larguraTabela) / 2 + cellWidth;;
    int startY = (screenHeight - alturaTabela) / 2;

    DrawText("TIPO",startX - 140,startY,20,WHITE);
    
    for(int col = 0;col < TOTAL_COLUNAS;col++){
        DrawText(nomesColunas[col],startX + (col * cellWidth),startY,20,WHITE); 
    }
    
    for(int lin = 0;lin < TOTAL_LINHAS;lin++){
        DrawText(nomesLinhas[lin],startX - 140,startY + ((lin + 1) * cellHeight),20,WHITE);
        
        for(int col = 0;col < TOTAL_COLUNAS;col++){
            float valor =score->matrizResultados[lin][col];
            if(col == COL_PRECISAO){
                DrawText(TextFormat("%.1f%%", valor),startX + (col * cellWidth),startY + ((lin + 1) * cellHeight),20,GREEN);
        } 

        else{
            DrawText(TextFormat("%.0f", valor),startX + (col * cellWidth),startY + ((lin + 1) * cellHeight),20,WHITE);
        }
    }
    } 

    float precisaoGeral =score->matrizResultados[LINHA_GERAL][COL_PRECISAO];
    float pulse =sinf(GetTime() * 8.0f) * 5.0f;
    int tamanhoRank = 120 + pulse;

    const char *ranking = "F";
    Color corRanking = RED;

    if (precisaoGeral >= 90){
        ranking = "S";
        corRanking = GOLD;
    }
    else if (precisaoGeral >= 80){
        ranking = "A";
        corRanking = PURPLE;
    }
    else if (precisaoGeral >= 70){
        ranking = "B";
        corRanking = BLUE;
    }
    else if (precisaoGeral >= 60){
        ranking = "C";
        corRanking = YELLOW;
    }
    else if (precisaoGeral >= 50){
        ranking = "D";
        corRanking = ORANGE;
    }

    DrawText("RANK",screenWidth - 220,100,30,GRAY);

    DrawText(ranking,screenWidth - 220,140,tamanhoRank,corRanking);

    const char *texto = "APERTE ENTER PARA VOLTAR AO MENU";

    int textoSize = 20;

    int larguraTexto = MeasureText(texto, textoSize);

    DrawText(texto,screenWidth - larguraTexto - 20, screenHeight - 40, textoSize, GRAY);
}

void atualizarResultados(GameState *gameState)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        *gameState = MENU;
    }
}

LinhasMatriz tipoParaLinha(Tiponota tipo)
{
    switch(tipo)
    {
        case NOTA_GRAVE:
            return LINHA_GRAVE;

        case NOTA_AGUDO:
            return LINHA_AGUDO;

        case NOTA_LONGA:
            return LINHA_LONGA;

        case NOTA_DIREITA:
        case NOTA_ESQUERDA:
            return LINHA_MOVIMENTO;

        default:
            return LINHA_GERAL;
    }
}

void calcularTotais(Score *score){
    for(int i = 0;i < total_de_notas;i++){
        LinhasMatriz linha =  tipoParaLinha(array_notas[i].tipo);

        score->matrizResultados[linha][COL_TOTAL]++;
        score->matrizResultados[LINHA_GERAL][COL_TOTAL]++;
}
}

void registrarAcerto(Score *score,Tiponota tipo){
    LinhasMatriz linha =  tipoParaLinha(tipo);

    score->matrizResultados[linha][COL_ACERTOS]++;
    score->matrizResultados[LINHA_GERAL][COL_ACERTOS]++;
}

void registrarmiss(Score *score,Tiponota tipo){
    LinhasMatriz linha =  tipoParaLinha(tipo);
    score->matrizResultados[linha][COL_MISS]++;
    score->matrizResultados[LINHA_GERAL][COL_MISS]++;
}

void calcularPrecisao(Score *score){
    for(int i=0;i<TOTAL_LINHAS;i++){
        float total = score->matrizResultados[i][COL_TOTAL];
        float acertos =score->matrizResultados[i][COL_ACERTOS];

        if(total > 0){
             score->matrizResultados[i][COL_PRECISAO] =(acertos / total) * 100.0f;
        }

         else{
            score->matrizResultados[i][COL_PRECISAO]=0.0f;
        }
    }
}
