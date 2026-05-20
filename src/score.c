#include "raylib.h"
#include "notas.h"
#include "score.h"
#include "buffs.h"
#include <stdio.h>
#include <math.h>

void init_sistema_pontos(Score *score){
    score->pontos = 0;
    score->streak = 0;
    score->maior_streak = 0;
    score->multiplicador = 1.0f;
    score->ultima_nota = JULG_PENDENTE;
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
void aumentar_streak(Score *score){
    score->streak++;
    
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

void draw_sistema_pontos(Score*score){
    const char *texto =
    HitToString(score->ultima_nota);

    int fonte = 40;

    int larguraTexto =
    MeasureText(texto, fonte);

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    DrawText(TextFormat("Pontuação: %d", score->pontos),20, 20, 30, WHITE);
    DrawText(TextFormat("%d", score->streak),20, 60, 30, WHITE);
    DrawText(TextFormat("%.0fx", score->multiplicador),20, 100, 30, WHITE);
    DrawText(HitToString(score->ultima_nota),screenWidth- larguraTexto - 20,screenHeight - 60,40,GREEN);
}