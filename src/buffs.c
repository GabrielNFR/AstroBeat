#include "buffs.h"
#include "raylib.h"
#include "notas.h"    
#include "player.h"    
#include "globals.h"   
#include <stdlib.h>    
#include <stdio.h>    
#include <math.h>     

Coletavel *listaColetaveis = NULL;

void inserirColetavel(float tempo, int lane, TipoBuff tipobuff, float duracao)
{
    Coletavel *novo = (Coletavel*)malloc(sizeof(Coletavel));

    if (novo == NULL)
    {
        printf("FALHA NA ALOCAÇÃO DE MEMÓRIA PARA NOVO ITEM COLETAVEL");
        return;
    }

    float posX[3] = {FAIXA_ESQUERDA, FAIXA_CENTRO, FAIXA_DIREITA};
    novo->posicao = (Vector3){posX[lane], 1.0f, -80.0f};
    novo->tipobuff = tipobuff;
    novo->duracao = duracao;
    novo->tempoSpawn = tempo;
    novo->lane = lane;
    novo->ativo = 0;
    novo->proximo = NULL;

    if (listaColetaveis == NULL)
    {
        listaColetaveis = novo;
    }
    else
    {
        Coletavel *atual = listaColetaveis;
        while (atual->proximo != NULL)
        {
            atual = atual->proximo;
        }
        atual->proximo = novo;
    }
}

void removerColetavel(Coletavel *anterior, Coletavel *atual)
{
    if (listaColetaveis == NULL)
    {
        printf("LISTA DE COLETAVEIS VAZIA");
        return;
    }

    if (anterior == NULL)
    {
        listaColetaveis = atual->proximo;
        free(atual);
    }
    else
    {
        anterior->proximo = atual->proximo;
        free(atual);
    }
}

void atualizarColetavel(float tempo_atual)
{
    Coletavel *atual = listaColetaveis;
    Coletavel *anterior = NULL;

    while (atual != NULL)
    {
        Coletavel *proximo = atual->proximo;

        if (!atual->ativo && tempo_atual >= atual->tempoSpawn - TEMPO_ATE_HIT)
        {
            atual->ativo = 1;
        }
        
        if (atual->ativo)
        {
            float z = -80.0f + (tempo_atual - (atual->tempoSpawn - TEMPO_ATE_HIT)) * VEL_NOTAS;
            if (z >= (15 - HIT_OFFSET))
            {
                removerColetavel(anterior, atual);
                atual = proximo;
                continue;
            }
        }
        anterior = atual;
        atual = proximo;
    }
}

void desenharColetavel(float tempo_atual)
{
    float posX[3] = {FAIXA_ESQUERDA, FAIXA_CENTRO, FAIXA_DIREITA};

    Coletavel *atual = listaColetaveis;
    while (atual != NULL)
    {
        if (!atual->ativo)
        {
            atual = atual->proximo;
            continue;
        }

        float x = posX[atual->lane];
        float z = -80.0f + (tempo_atual - (atual->tempoSpawn - TEMPO_ATE_HIT)) * VEL_NOTAS;
        if (z >= (15 - HIT_OFFSET) || z <= -80.0f)
        {
            atual = atual->proximo;
            continue;
        }

        Color cor = (atual->tipobuff == BUFF_MULTIPLICADOR) ? GOLD : PURPLE;
        float y = 1.0f + sinf(tempo_atual * 4.0f) * 0.2f;

        DrawSphere((Vector3){x, y, z}, 0.3f, Fade(cor, 0.7f));

        atual = atual->proximo;
    }
}

void verificarColisao(Nave *jogador, float tempo_atual)
{
    float posX[3] = {FAIXA_ESQUERDA, FAIXA_CENTRO, FAIXA_DIREITA};
    Coletavel *atual = listaColetaveis;
    Coletavel *anterior = NULL;
    
    while (atual != NULL)
    {
        Coletavel *proximo = atual->proximo;

        if (!atual->ativo)
        {
            anterior = atual;
            atual = proximo;
            continue;
        }

        float x = posX[atual->lane];
        float z = -80.0f + (tempo_atual - (atual->tempoSpawn - TEMPO_ATE_HIT)) * VEL_NOTAS;

        BoundingBox hitboxColetavel = {
            {x - 0.3f, 1.0f - 0.3f, z - 0.3f},
            {x + 0.3f, 1.0f + 0.3f, z + 0.3f}
        };

        if (CheckCollisionBoxes(jogador->hitbox, hitboxColetavel))
        {
            if (atual->tipobuff == BUFF_MULTIPLICADOR)
            {
                jogador->buffMultiplicador = true;
                jogador->tempoFimMult = tempo_atual + atual->duracao;
                printf("BUFF COLETADO: MULTIPLICADOR 2x por %.1fs\n", atual->duracao);
                fflush(stdout);
            }
            else if (atual->tipobuff == BUFF_JANELA)
            {
                jogador->buffJanela = true;
                jogador->tempoFimJanela = tempo_atual + atual->duracao;
                printf("BUFF COLETADO: JANELA +20ms por %.1fs\n", atual->duracao);
                fflush(stdout);
            }
            removerColetavel(anterior, atual);
            atual = proximo;
            continue;
        }
        anterior = atual;
        atual = proximo;
    }
}

void limparColetavel(void)
{
    Coletavel *atual = listaColetaveis;
    while (atual != NULL)
    {
        Coletavel *proximo = atual->proximo;
        free(atual);
        atual = proximo;    
    }
    listaColetaveis = NULL;
}