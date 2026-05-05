#include "audio.h"
#include <stdio.h>

bool inicializarAudio(Audio *audio, const char *caminhoMusica)
{
    *audio = (Audio){0};

    InitAudioDevice();
    if (!IsAudioDeviceReady()) {
        printf("Erro ao inicializar dispositivo de audio\n");
        return false;
    }

    audio->musica = LoadMusicStream(caminhoMusica);
    if (audio->musica.stream.buffer == NULL) {
        printf("Erro ao carregar musica: %s\n", caminhoMusica);
        CloseAudioDevice();
        return false;
    }

    audio->musica.looping = false;
    audio->musicaCarregada = true;
    audio->volumeMusica = 0.8f;
    SetMusicVolume(audio->musica, audio->volumeMusica);

    return true;
}

void atualizarAudio(Audio *audio)
{
    if (audio->musicaCarregada) {
        UpdateMusicStream(audio->musica);
    }
}

void iniciarMusica(Audio *audio)
{
    if (!audio->musicaCarregada) return;

    StopMusicStream(audio->musica);
    PlayMusicStream(audio->musica);
}

void pausarMusica(Audio *audio)
{
    if (audio->musicaCarregada) {
        PauseMusicStream(audio->musica);
    }
}

void continuarMusica(Audio *audio)
{
    if (audio->musicaCarregada) {
        ResumeMusicStream(audio->musica);
    }
}

void pararMusica(Audio *audio)
{
    if (audio->musicaCarregada) {
        StopMusicStream(audio->musica);
    }
}

float obterTempoMusica(Audio *audio)
{
    if (!audio->musicaCarregada) return 0.0f;

    return GetMusicTimePlayed(audio->musica);
}

void seekMusica(Audio *audio, float posicaoSegundos)
{
    if (!audio->musicaCarregada) return;

    bool estavaTocando = IsMusicStreamPlaying(audio->musica);
    SeekMusicStream(audio->musica, posicaoSegundos);
    // SeekMusicStream para a música; retoma se estava tocando
    if (estavaTocando) {
        PlayMusicStream(audio->musica);
    }
}

void descarregarAudio(Audio *audio)
{
    if (audio->musicaCarregada) {
        UnloadMusicStream(audio->musica);
        audio->musicaCarregada = false;
    }

    if (IsAudioDeviceReady()) {
        CloseAudioDevice();
    }
}
