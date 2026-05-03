#include "raylib.h"
#include "notas.h"
#include <stdio.h>
Notas array_notas[1000];
int total_de_notas;

void leitura_arquivo_musica(){
    int i= 0;

    FILE *f = fopen("mapas/mapaElektronomia.txt", "r");
    if (f==NULL){
        printf("erro ao abrir o arquivo");
        return;
    }
    
    
    while(i < 1000 && fscanf(f,"%f %d %d",&array_notas[i].tempo, &array_notas[i].lane, (int*)&array_notas[i].tipo)==3){
        array_notas[i].ativa=0;
        array_notas[i].isHit=0;
        array_notas[i].pontos=100;
        printf("Nota %d: tempo=%.2f lane=%d tipo=%d\n",
       i,
       array_notas[i].tempo,
       array_notas[i].lane,
       array_notas[i].tipo);
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
    float posX[3] = {-2.0f, 0.0f, 2.0f};
    for (int i=0;i<total_de_notas;i++){

        if(1){
            if (array_notas[i].lane < 0 || array_notas[i].lane > 2) continue;
    
            float x = posX[array_notas[i].lane];
            float z = -80.0f + (tempo_atual - array_notas[i].tempo) * VEL_NOTAS;
    
            // só desenha se estiver na tela
            if (z < 15.0f && z > -80.0f){
                switch (array_notas[i].tipo) {
                    case NOTA_GRAVE:
                        DrawCube((Vector3){x, 0.5f, z}, 0.5f, 0.5f, 0.5f, BLUE);
                        break;
                    case NOTA_AGUDO:
                        DrawCube((Vector3){x, 0.5f, z}, 0.5f, 0.5f, 0.5f, RED);
                        break;
                    case NOTA_LONGA:
                        DrawCube((Vector3){x, 0.5f, z}, 0.5f, 0.5f, 0.5f, WHITE);
                        break;
                    case NOTA_DIREITA:
                        DrawCube((Vector3){x, 0.5f, z}, 0.5f, 0.5f, 0.5f, GREEN);
                        break;
                    case NOTA_ESQUERDA:
                        DrawCube((Vector3){x, 0.5f, z}, 0.5f, 0.5f, 0.5f, YELLOW);
                        break;
                }
            }
        }
    }       
}

void verificarAcertos(Nave *jogador, float tempo_atual)
{
    float posX[3] = {FAIXA_ESQUERDA, FAIXA_CENTRO, FAIXA_DIREITA};
    
    for (int i = 0; i < total_de_notas; i++)
    {
        if (!array_notas[i].ativa || array_notas[i].isHit) continue;

        float x = posX[array_notas[i].lane];
        float z = -80.0f + (tempo_atual - array_notas[i].tempo) * VEL_NOTAS;

        BoundingBox notaBox = {
            {x - 0.25f, 0.25f, z - 0.25f},
            {x + 0.25f, 0.75f, z + 0.25f}
        };

        if (!CheckCollisionBoxes(jogador->hitbox, notaBox)) continue;

        int acertou = 0;
        switch (array_notas[i].tipo) {
            case NOTA_GRAVE:
                acertou = IsKeyPressed(KEY_K);
                break;
            case NOTA_AGUDO:
                acertou = IsKeyPressed(KEY_J);
                break;
            case NOTA_LONGA:
                acertou = IsKeyPressed(KEY_SPACE);
                break;
            case NOTA_DIREITA:
                acertou = IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT);
                break;
            case NOTA_ESQUERDA:
                acertou = IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT);
                break;
        }

        if (acertou) {
            array_notas[i].isHit = 1;
            printf("Acertou nota %d! tipo=%d lane=%d\n", i, array_notas[i].tipo, array_notas[i].lane);
        }
    }
}

