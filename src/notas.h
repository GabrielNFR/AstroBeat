#ifndef NOTAS_H
#define NOTAS_H

#include "globals.h"

typedef struct Nave Nave;

#define MAX_NOTAS 1000
#define VEL_NOTAS 10.0f
#define HIT_OFFSET 2.0f // offset a partir do z 0.0f
#define TEMPO_ATE_HIT ((80.0f - HIT_OFFSET) / VEL_NOTAS)

typedef struct{
    float tempo;
    float duracao;
    float consumo;
    float tempoAcerto;
    Tiponota tipo;
    Julgamento resultado;
    int lane;
    int ativa;
    int finalizada;
    int pontos;
    int holding;
    int mostrarEfeito;
}Notas;

extern Notas array_notas[MAX_NOTAS];
extern int total_de_notas;
extern float JANELA_PERFECT;
extern float JANELA_GREAT;
extern float JANELA_GOOD;
extern float JANELA_OK;
extern float JANELA_MISS;

void leitura_arquivo_musica(const char *caminho);
void leitura_arquivo_coletaveis(const char *caminho);
void atualizar_notas(float tempo_atual);
void desenhar_notas(float tempo_atual);
void resetar_notas(void);
void verificarAcertos(Nave *jogador,Score*score, float tempo_atual, float deltaTime);

#endif
