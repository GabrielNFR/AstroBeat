#include "raylib.h"
#include "notas.h"
#include <stdio.h>
#include <math.h>

Notas array_notas[1000];
int total_de_notas;

static bool TeclaPressionadaParaTipo(Tiponota tipo)
{
    switch (tipo) {
        case NOTA_GRAVE:    return IsKeyPressed(KEY_K);
        case NOTA_AGUDO:    return IsKeyPressed(KEY_J);
        case NOTA_DIREITA:  return IsKeyPressed(KEY_D);
        case NOTA_ESQUERDA: return IsKeyPressed(KEY_A);
        case NOTA_LONGA:    return IsKeyPressed(KEY_SPACE);
        default:            return false;
    }
}

static bool TeclaSeguradaParaTipo(Tiponota tipo)
{
    switch (tipo) {
        case NOTA_LONGA: return IsKeyDown(KEY_SPACE);
        default:         return false;
    }
}

void leitura_arquivo_musica(){
    int i= 0;

    FILE *f = fopen("mapas/mapaElektronomia.txt", "r");
    if (f==NULL){
        printf("erro ao abrir o arquivo");
        return;
    }


    while(i < 1000 && fscanf(f,"%f %d %d %f",&array_notas[i].tempo, &array_notas[i].lane, (int*)&array_notas[i].tipo, &array_notas[i].duracao)==4){
        array_notas[i].ativa=0;
        array_notas[i].finalizada=0;
        array_notas[i].resultado = JULG_PENDENTE;
        array_notas[i].holding = 0;
        array_notas[i].consumo = 0;
        array_notas[i].pontos=100;
        printf("Nota %d: tempo=%.2f lane=%d tipo=%d dur=%.2f\n\n",
       i,
       array_notas[i].tempo,
       array_notas[i].lane,
       array_notas[i].tipo,
       array_notas[i].duracao);
        i++;
    }

    total_de_notas=i;
    printf("Total de notas: %d\n", total_de_notas);
    fclose(f);
}

void atualizar_notas( float tempo_atual){
    for (int i=0;i<total_de_notas;i++){
        if(!array_notas[i].ativa && tempo_atual>=array_notas[i].tempo){
            array_notas[i].ativa=1;
        }
    }
}

void desenhar_notas(float tempo_atual){
    float posX[3] = {FAIXA_ESQUERDA, FAIXA_CENTRO, FAIXA_DIREITA};
    for (int i = 0; i < total_de_notas; i++) {
    if (array_notas[i].lane < 0 || array_notas[i].lane > 2) continue;
    if (!array_notas[i].ativa || array_notas[i].finalizada) continue;

    float x = posX[array_notas[i].lane];
    float z = -80.0f + (tempo_atual - array_notas[i].tempo) * VEL_NOTAS;

    // Verifica se a nota está na tela (notas longas olham cabeça e cauda)
    if (array_notas[i].tipo == NOTA_LONGA && array_notas[i].duracao > 0.0f) {
        float zTail = z - array_notas[i].duracao * VEL_NOTAS;
        if (z > 15.0f && zTail > 15.0f) continue;
        if (z < -80.0f && zTail < -80.0f) continue;
    } else {
        if (z >= 15.0f || z <= -80.0f) continue;
    }

    // Desenha conforme o tipo
    switch (array_notas[i].tipo) {
        case NOTA_GRAVE:
            DrawCube((Vector3){x, 0.5f, z}, 0.5f, 0.5f, 0.5f, BLUE);
            break;
        case NOTA_AGUDO:
            DrawCube((Vector3){x, 0.5f, z}, 0.5f, 0.5f, 0.5f, RED);
            break;
        case NOTA_LONGA:
        {
            float zBodyTotal = array_notas[i].duracao * VEL_NOTAS;

            if (array_notas[i].holding == 1 || array_notas[i].holding == -1)
            {
                float consumo_visual = array_notas[i].consumo * VEL_NOTAS;
                float zBodyRestante = zBodyTotal - consumo_visual;
                if (zBodyRestante <= 0.0f) break;  // totalmente consumida

                // Consome da frente (cabeça) para trás (cauda)
                float z_cauda   = z - zBodyTotal;           // cauda original, não mexe
                float z_frente  = z - consumo_visual;       // frente avança em direção à cauda
                float zCenter   = (z_frente + z_cauda) / 2.0f;

                DrawCube((Vector3){x, 0.5f, zCenter}, 0.5f, 0.5f, zBodyRestante, WHITE);
            }
            else
            {
                float zCenter = z - (zBodyTotal / 2.0f);
                DrawCube((Vector3){x, 0.5f, zCenter}, 0.5f, 0.5f, zBodyTotal, WHITE);
            }
            break;
        }
        case NOTA_DIREITA:
            DrawCube((Vector3){x, 0.5f, z}, 0.5f, 0.5f, 0.5f, GREEN);
            break;
        case NOTA_ESQUERDA:
            DrawCube((Vector3){x, 0.5f, z}, 0.5f, 0.5f, 0.5f, YELLOW);
            break;
        }
    }
}

void resetar_notas(void)
{
    for (int i = 0; i < total_de_notas; i++) {
        array_notas[i].ativa = 0;
        array_notas[i].finalizada = 0;
        array_notas[i].resultado = JULG_PENDENTE;
        array_notas[i].holding = 0;
        array_notas[i].consumo = 0.0f;
        array_notas[i].pontos = 100;
    }
}

void verificarAcertos(Nave *jogador, float tempo_atual, float deltaTime)
{
    for (int i = 0; i < total_de_notas; i++)
    {
        if (!array_notas[i].ativa || array_notas[i].finalizada)
            continue;

        float tempo_hit_ideal = array_notas[i].tempo + TEMPO_ATE_HIT;
        float diferenca_tempo = tempo_atual - tempo_hit_ideal;
        float diferenca_absoluta = fabsf(diferenca_tempo);
        float z_nota = -80.0f + (tempo_atual - array_notas[i].tempo) * VEL_NOTAS;

        // MISS POR SAIR DA TELA
        if (array_notas[i].tipo == NOTA_LONGA)
        {
            float z_tail = z_nota - array_notas[i].duracao * VEL_NOTAS;
            if (z_tail >= 15.0f && array_notas[i].holding != 1)
            {
                array_notas[i].finalizada = 1;
                array_notas[i].resultado = JULG_MISS;
                printf("LONG MISS (cauda saiu da tela)\n");
                fflush(stdout);
                continue;
            }
        }
        else
        {
            if (z_nota >= 15.0f)
            {
                array_notas[i].finalizada = 1;
                array_notas[i].resultado = JULG_MISS;
                printf("MISS (saiu da tela)\n");
                fflush(stdout);
                continue;
            }
        }

        // JANELA DE HIT (normal + cabeça da longa)
        if (diferenca_absoluta <= JANELA_MISS)
        {
            if (array_notas[i].tipo != NOTA_LONGA)
            {
                int mesma_lane = (jogador->laneAnterior == array_notas[i].lane);
                if (mesma_lane && TeclaPressionadaParaTipo(array_notas[i].tipo))
                {
                    array_notas[i].finalizada = 1;

                    if (diferenca_absoluta <= JANELA_PERFECT) {
                        array_notas[i].resultado = JULG_PERFECT;
                        printf("PERFECT\n");
                    } else if (diferenca_absoluta <= JANELA_GREAT) {
                        array_notas[i].resultado = JULG_GREAT;
                        printf("GREAT\n");
                    } else if (diferenca_absoluta <= JANELA_GOOD) {
                        array_notas[i].resultado = JULG_GOOD;
                        printf("GOOD\n");
                    } else if (diferenca_absoluta <= JANELA_OK) {
                        array_notas[i].resultado = JULG_OK;
                        printf("OK\n");
                    }
                    fflush(stdout);
                }
            }
            else if (array_notas[i].holding == 0)
            {
                int mesma_lane = (jogador->laneAnterior == array_notas[i].lane);
                if (mesma_lane && TeclaPressionadaParaTipo(NOTA_LONGA))
                {
                    array_notas[i].holding = 1;

                    if (diferenca_absoluta <= JANELA_PERFECT) {
                        array_notas[i].resultado = JULG_PERFECT;
                        printf("LONG HEAD PERFECT\n");
                    } else if (diferenca_absoluta <= JANELA_GREAT) {
                        array_notas[i].resultado = JULG_GREAT;
                        printf("LONG HEAD GREAT\n");
                    } else if (diferenca_absoluta <= JANELA_GOOD) {
                        array_notas[i].resultado = JULG_GOOD;
                        printf("LONG HEAD GOOD\n");
                    } else if (diferenca_absoluta <= JANELA_OK) {
                        array_notas[i].resultado = JULG_OK;
                        printf("LONG HEAD OK\n");
                    }
                    fflush(stdout);
                }
            }
        }

        // HOLD: roda todo frame enquanto segurada
        if (array_notas[i].tipo == NOTA_LONGA && array_notas[i].holding == 1)
        {
            float tempo_fim = array_notas[i].tempo + TEMPO_ATE_HIT + array_notas[i].duracao;

            if (!TeclaSeguradaParaTipo(NOTA_LONGA))
            {
                array_notas[i].holding = -1;
                printf("LONG ABANDONADA\n");
                fflush(stdout);
            }
            else if (tempo_atual >= tempo_fim)
            {
                array_notas[i].finalizada = 1;
                printf("LONG COMPLETA\n");
                fflush(stdout);
            }
            else
            {
                array_notas[i].consumo += deltaTime;
                if (array_notas[i].consumo > array_notas[i].duracao)
                {
                    array_notas[i].consumo = array_notas[i].duracao;
                }
            }
        }
    }
}
