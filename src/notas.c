#include "raylib.h"
#include "notas.h"
#include "score.h"
#include "player.h"
#include "buffs.h"
#include "notas_modelos.h"
#include <stdio.h>
#include <math.h>

Notas array_notas[1000];
int total_de_notas;

float JANELA_PERFECT = 0.050f;
float JANELA_GREAT   = 0.075f;
float JANELA_GOOD    = 0.100f;
float JANELA_OK      = 0.125f;
float JANELA_MISS    = 0.150f;

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

void leitura_arquivo_musica(const char *caminho){
    int i= 0;

    if (caminho == NULL)
    {
        total_de_notas = 0;
        return;
    }

    FILE *f = fopen(caminho, "r");
    if (f==NULL){
        printf("Erro ao abrir o arquivo: %s\n", caminho);
        total_de_notas = 0;
        return;
    }

    while(i < 1000 && fscanf(f,"%f %d %d %f",&array_notas[i].tempo, &array_notas[i].lane, (int*)&array_notas[i].tipo, &array_notas[i].duracao)==4){
        array_notas[i].ativa=0;
        array_notas[i].finalizada=0;
        array_notas[i].resultado = JULG_PENDENTE;
        array_notas[i].holding = 0;
        array_notas[i].consumo = 0;
        array_notas[i].pontos=100;
        //printf("Nota %d: tempo=%.2f lane=%d tipo=%d dur=%.2f\n\n",
       //i,
       //array_notas[i].tempo,
       //array_notas[i].lane,
       //array_notas[i].tipo,
       //array_notas[i].duracao);
        i++;
    }

    total_de_notas=i;
    printf("Total de notas: %d\n", total_de_notas);
    fclose(f);
}

void leitura_arquivo_coletaveis(const char *caminho)
{
    if (caminho == NULL)
    {
        return;
    }

    FILE *f = fopen(caminho, "r");
    if (f == NULL) {
        printf("Coletaveis: arquivo não encontrado\n");
        return;
    }
    printf("Coletaveis: arquivo aberto\n");

    float tempo, duracao;
    int lane, tipoBuff;
    int contador = 0;
    while (fscanf(f, "%f %d %d %f", &tempo, &lane, &tipoBuff, &duracao) == 4)
    {
        inserirColetavel(tempo, lane, (TipoBuff)tipoBuff, duracao);
        contador++;
    }
    printf("coletaveis: %d lidos\n", contador);
    fclose(f);
}
void atualizar_notas( float tempo_atual){
    for (int i=0;i<total_de_notas;i++){
        if(!array_notas[i].ativa && tempo_atual >= array_notas[i].tempo - TEMPO_ATE_HIT){
            array_notas[i].ativa=1;
        }
    }
}

void desenhar_notas(float tempo_atual){
    float posX[3] = {FAIXA_ESQUERDA, FAIXA_CENTRO, FAIXA_DIREITA};
    for (int i = 0; i < total_de_notas; i++) {
    if (array_notas[i].lane < 0 || array_notas[i].lane > 2) continue;

    float x = posX[array_notas[i].lane];
    float z = -80.0f + (tempo_atual - (array_notas[i].tempo - TEMPO_ATE_HIT)) * VEL_NOTAS;

    if (array_notas[i].ativa && !array_notas[i].finalizada)
    {
        bool naTela = true;
        // Verifica se a nota está na tela (notas longas olham cabeça e cauda)
        if (array_notas[i].tipo == NOTA_LONGA && array_notas[i].duracao > 0.0f) {
            float zTail = z - array_notas[i].duracao * VEL_NOTAS;
            if (z >= 15.0f - HIT_OFFSET && zTail >= 15.0f - HIT_OFFSET) naTela = false;
            if (z <= -80.0f && zTail <= -80.0f) naTela = false;
        } else {
            if (z >= 15.0f - HIT_OFFSET || z <= -80.0f) naTela = false;
        }

        if (naTela)
        {
            // Desenha conforme o tipo
            switch (array_notas[i].tipo) {
                case NOTA_GRAVE:
                    desenharNotaGrave(x, z);
                    break;
                case NOTA_AGUDO:
                    desenharNotaAgudo(x, z);
                    break;
                case NOTA_LONGA:
                {
                    float zBodyTotal = array_notas[i].duracao * VEL_NOTAS;

                    if (array_notas[i].holding == 1 || array_notas[i].holding == -1)
                    {
                        float consumo_visual = array_notas[i].consumo * VEL_NOTAS;
                        float zBodyRestante = zBodyTotal - consumo_visual;
                        if (zBodyRestante <= 0.0f) break; // totalmente consumida

                        // Consome da frente (cabeça) para trás (cauda)
                        float z_cauda   = z - zBodyTotal;  // cauda original, não mexe
                        float z_frente  = z - consumo_visual; // frente avança em direção à cauda
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
                    desenharNotaDireita(x, z);
                    break;
                case NOTA_ESQUERDA:
                    desenharNotaEsquerda(x, z);
                    break;
            }
        }
    }

    if (array_notas[i].mostrarEfeito == 1)
    {
        float tempoDecorrido = tempo_atual - array_notas[i].tempoAcerto;
        float duracaoTotal = 0.4f;

        if (tempoDecorrido <= duracaoTotal)
        {
            Color corOnda;
            if (array_notas[i].tipo == NOTA_GRAVE)      corOnda = BLUE;
            else if (array_notas[i].tipo == NOTA_AGUDO)    corOnda = RED;
            else if (array_notas[i].tipo == NOTA_DIREITA)  corOnda = GREEN;
            else if (array_notas[i].tipo == NOTA_ESQUERDA) corOnda = YELLOW;
            else                                         corOnda = WHITE;

            float zAcertoFixo = -HIT_OFFSET;
            desenharEfeitoAcerto(x, zAcertoFixo, tempoDecorrido, duracaoTotal, corOnda);
        }
        else
        {
            array_notas[i].mostrarEfeito = 0;
        }
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
        array_notas[i].mostrarEfeito = 0;
        array_notas[i].tempoAcerto = 0.0f;
    }
}

void verificarAcertos(Nave *jogador,Score *score, float tempo_atual, float deltaTime)
{
    bool hitPorTipo[5] = {0};

    float janela_perfect = JANELA_PERFECT + (jogador->buffJanela ? 0.020f : 0.0f);
    float janela_great   = JANELA_GREAT   + (jogador->buffJanela ? 0.020f : 0.0f);
    float janela_good    = JANELA_GOOD    + (jogador->buffJanela ? 0.020f : 0.0f);
    float janela_ok      = JANELA_OK      + (jogador->buffJanela ? 0.020f : 0.0f);
    float janela_miss    = JANELA_MISS    + (jogador->buffJanela ? 0.020f : 0.0f);
    
    for (int i = 0; i < total_de_notas; i++)
    {
        if (!array_notas[i].ativa || array_notas[i].finalizada)
            continue;

        float tempo_hit_ideal = array_notas[i].tempo;
        float diferenca_tempo = tempo_atual - tempo_hit_ideal;
        float diferenca_absoluta = fabsf(diferenca_tempo);
        float z_nota = -80.0f + (tempo_atual - array_notas[i].tempo) * VEL_NOTAS;

        // MISS POR SAIR DA TELA
        if (array_notas[i].tipo == NOTA_LONGA)
        {
            float z_tail = z_nota - array_notas[i].duracao * VEL_NOTAS;
            if (z_tail >= 15.0f - HIT_OFFSET && array_notas[i].holding != 1)
            {
                array_notas[i].finalizada = 1;
                array_notas[i].resultado = JULG_MISS;
                registrarmiss(score,array_notas[i].tipo);
                errar_nota(score);
                printf("LONG MISS (cauda saiu da tela)\n");
                fflush(stdout);
                continue;
            }
        }
        else
        {
            if (diferenca_tempo > 0.6f)
            {
                array_notas[i].finalizada = 1;
                array_notas[i].resultado = JULG_MISS;
                registrarmiss(score,array_notas[i].tipo);
                errar_nota(score);
                printf("MISS (saiu da tela)\n");
                fflush(stdout);
                continue;
            }
        }

        // JANELA DE HIT (normal + cabeça da longa)
        if (diferenca_absoluta <= janela_miss)
        {
            if (array_notas[i].tipo != NOTA_LONGA)
            {
                if (hitPorTipo[array_notas[i].tipo]) continue;
                
                int mesma_lane = (jogador->laneAnterior == array_notas[i].lane);
                if (mesma_lane && TeclaPressionadaParaTipo(array_notas[i].tipo))
                {
                    hitPorTipo[array_notas[i].tipo] = true;
                    array_notas[i].finalizada = 1;
                    array_notas[i].mostrarEfeito = 1;
                    array_notas[i].tempoAcerto = tempo_atual;
                    array_notas[i].pontos = jogador->buffMultiplicador ? 200 : 100;
                    if (jogador->buffMultiplicador) printf("2x PONTOS\n");

                    if (diferenca_absoluta <= janela_perfect) {
                        array_notas[i].resultado = JULG_PERFECT;
                        registrarAcerto(score,array_notas[i].tipo);
                        aumentar_streak(score);
                        adicionar_pontos(score,JULG_PERFECT, array_notas[i].pontos);
                        printf("PERFECT\n");
                    } else if (diferenca_absoluta <= janela_great) {
                        array_notas[i].resultado = JULG_GREAT;
                        registrarAcerto(score,array_notas[i].tipo);
                        aumentar_streak(score);
                        adicionar_pontos(score,JULG_GREAT, array_notas[i].pontos);
                        printf("GREAT\n");
                    } else if (diferenca_absoluta <= janela_good) {
                        array_notas[i].resultado = JULG_GOOD;
                        registrarAcerto(score,array_notas[i].tipo);
                        aumentar_streak(score);
                        adicionar_pontos(score,JULG_GOOD, array_notas[i].pontos);
                        printf("GOOD\n");
                    } else if (diferenca_absoluta <= janela_ok) {
                        array_notas[i].resultado = JULG_OK;
                        registrarAcerto(score,array_notas[i].tipo);
                        aumentar_streak(score);
                        adicionar_pontos(score,JULG_OK, array_notas[i].pontos);
                        printf("OK\n");
                    }
                    fflush(stdout);
                }
            }
            else if (array_notas[i].holding == 0)
            {
                if (hitPorTipo[NOTA_LONGA]) continue;
                
                int mesma_lane = (jogador->laneAnterior == array_notas[i].lane);
                if (mesma_lane && TeclaPressionadaParaTipo(NOTA_LONGA))
                {
                    hitPorTipo[array_notas[i].tipo] = true;
                    array_notas[i].holding = 1;
                    array_notas[i].tickTimer = 0.0f;
                    array_notas[i].pontos = jogador->buffMultiplicador ? 200 : 100;
                    if (jogador->buffMultiplicador) printf("2x PONTOS (LONG)\n");

                    if (diferenca_absoluta <= janela_perfect) {
                        array_notas[i].resultado = JULG_PERFECT;
                        registrarAcerto(score,array_notas[i].tipo);
                        aumentar_streak(score);
                        adicionar_pontos(score,JULG_PERFECT, array_notas[i].pontos);
                        printf("LONG HEAD PERFECT\n");
                    } else if (diferenca_absoluta <= janela_great) {
                        array_notas[i].resultado = JULG_GREAT;
                        registrarAcerto(score,array_notas[i].tipo);
                        aumentar_streak(score);
                        adicionar_pontos(score,JULG_GREAT, array_notas[i].pontos);
                        printf("LONG HEAD GREAT\n");
                    } else if (diferenca_absoluta <= janela_good) {
                        array_notas[i].resultado = JULG_GOOD;
                        registrarAcerto(score,array_notas[i].tipo);
                        aumentar_streak(score);
                        adicionar_pontos(score,JULG_GOOD, array_notas[i].pontos);
                        printf("LONG HEAD GOOD\n");
                    } else if (diferenca_absoluta <= janela_ok) {
                        array_notas[i].resultado = JULG_OK;
                        registrarAcerto(score,array_notas[i].tipo);
                        aumentar_streak(score);
                        adicionar_pontos(score,JULG_OK, array_notas[i].pontos);
                        printf("LONG HEAD OK\n");
                    }
                    fflush(stdout);
                }
            }
        }

        // HOLD: roda todo frame enquanto segurada
        if (array_notas[i].tipo == NOTA_LONGA && array_notas[i].holding == 1)
        {
            float tempo_fim = array_notas[i].tempo + array_notas[i].duracao;

            if (!TeclaSeguradaParaTipo(NOTA_LONGA))
            {
                array_notas[i].holding = -1;
                errar_nota(score);
                printf("LONG ABANDONADA\n");
                fflush(stdout);
            }
            else if (tempo_atual >= tempo_fim)
            {
                array_notas[i].finalizada = 1;
                aumentar_streak(score);
                adicionar_pontos(score,JULG_GREAT,50);
                printf("LONG COMPLETA\n");
                fflush(stdout);
            }
            else
            {
                array_notas[i].consumo += deltaTime;
                array_notas[i].tickTimer += deltaTime;

                if(array_notas[i].tickTimer >= 0.1f){
                    array_notas[i].tickTimer = 0.0f;
                    score->pontos +=10 * score->multiplicador;
                }
                }

                if (array_notas[i].consumo > array_notas[i].duracao)
                {
                    array_notas[i].consumo = array_notas[i].duracao;
                }
            }
        }
    }


