#ifndef AUDIO_H
#define AUDIO_H

#include "raylib.h"
#include <stdbool.h>

typedef struct {
    Music musica;
    bool musicaCarregada;
    float volumeMusica;
} Audio;

bool inicializarAudio(Audio *audio, const char *caminhoMusica);
void atualizarAudio(Audio *audio);
void iniciarMusica(Audio *audio);
void pausarMusica(Audio *audio);
void continuarMusica(Audio *audio);
void pararMusica(Audio *audio);
float obterTempoMusica(Audio *audio);
float carregarVolumeConfigurado(void);
void definirVolumeMusica(Audio *audio, float volume);
void seekMusica(Audio *audio, float posicaoSegundos);
void descarregarAudio(Audio *audio);

#endif
