#ifndef NOTAS_H
#define NOTAS_H

typedef struct Nave Nave;

#define MAX_NOTAS 1000
#define VEL_NOTAS 10.0f
#define HIT_OFFSET 2.0f // offset a partir do z 0.0f
#define TEMPO_ATE_HIT ((80.0f - HIT_OFFSET) / VEL_NOTAS)

typedef enum{
    NOTA_GRAVE = 0,
    NOTA_AGUDO = 1,
    NOTA_LONGA = 2,
    NOTA_DIREITA = 3,
    NOTA_ESQUERDA = 4
} Tiponota;

typedef enum{
    JULG_PENDENTE = 0,
    JULG_PERFECT,
    JULG_GREAT,
    JULG_GOOD,
    JULG_OK,
    JULG_MISS
}Julgamento;

typedef struct{
    float tempo;
    float duracao;
    float consumo;
    Tiponota tipo;
    Julgamento resultado;
    int lane;
    int ativa;
    int finalizada;
    int pontos;
    int holding;
}Notas;

extern Notas array_notas[MAX_NOTAS];
extern int total_de_notas;
extern float JANELA_PERFECT;
extern float JANELA_GREAT;
extern float JANELA_GOOD;
extern float JANELA_OK;
extern float JANELA_MISS;

void leitura_arquivo_musica(void);
void leitura_arquivo_coletaveis(void);
void atualizar_notas(float tempo_atual);
void desenhar_notas(float tempo_atual);
void resetar_notas(void);
void verificarAcertos(Nave *jogador, float tempo_atual, float deltaTime);

#endif
