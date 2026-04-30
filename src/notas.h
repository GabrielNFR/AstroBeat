#ifndef NOTAS_H
#define NOTAS_H
#define MAX_NOTAS 1000


typedef enum{
    NOTA_NORMAL = 0,
    NOTA_LONGA = 1,
    NOTA_DIREITA = 2,
    NOTA_ESQUERDA = 3

} Tiponota;

typedef struct{
    float tempo;
    int lane;
    Tiponota tipo;
    int ativa;
    float duracao;
    int pontos;
}Notas;

extern Notas array_notas[MAX_NOTAS];
extern int total_de_notas;

void leitura_arquivo_musica(void);
void atualizar_notas(float tempo_atual);
void desenhar_notas(float tempo_atual);


#endif