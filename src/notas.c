#include "raylib.h"
#include "notas.h"
#include <stdio.h>
Notas array_notas[1000];
int total_de_notas;

void leitura_arquivo_musica(){
    int i= 0;

    FILE *f = fopen("assets/notas_teste.txt", "r");
    if (f==NULL){
        printf("erro ao abrir o arquivo");
        return;
    }
    
    
    while(i < 1000 && fscanf(f,"%f %d %d",&array_notas[i].tempo, &array_notas[i].lane, (int*)&array_notas[i].tipo)==3){
        array_notas[i].ativa=0;
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
    float velocidade = 10.0f;
    for (int i=0;i<total_de_notas;i++){

        if(1){
            if (array_notas[i].lane < 0 || array_notas[i].lane > 2) continue;
    
            float x = posX[array_notas[i].lane];
            float z = -80.0f + (tempo_atual - array_notas[i].tempo) * velocidade;
    
            // só desenha se estiver na tela
            if (z < 15.0f && z > -20.0f){
                DrawCube((Vector3){x, 0.5f, z}, 0.5f, 0.5f, 0.5f, RED);
            }
            
        }


        }
        
    }

