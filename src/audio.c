#include "audio.h"
#include <raylib.h>
#include <stdio.h>
#define ARQUIVO_CONFIG "configuracoes.cfg"
#define VOLUME_PADRAO 0.8f

static float limitarVolume(float volume)
{
    if (volume < 0.0f) return 0.0f;
    if (volume > 1.0f) return 1.0f;
    return volume;
}

static float carregarVolumeSalvo(void)
{
    FILE *f = fopen(ARQUIVO_CONFIG, "r");
    float volume = VOLUME_PADRAO;

    if (f != NULL) {
        if (fscanf(f, "%f", &volume) != 1) {
            volume = VOLUME_PADRAO;
        }

        fclose(f);
    }
    return limitarVolume(volume);
}

float carregarVolumeConfigurado(void)
{
    return carregarVolumeSalvo();
}

static void salvarVolume(float volume)
{
    FILE *f = fopen(ARQUIVO_CONFIG, "w");

    if (f == NULL) {
        return;
    }

    fprintf(f, "%.2f\n", limitarVolume(volume));
    fclose(f);
}

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
    audio->volumeMusica = carregarVolumeSalvo();
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

void definirVolumeMusica(Audio *audio, float volume)
{
    audio->volumeMusica = limitarVolume(volume);

    if (audio->musicaCarregada) {
        SetMusicVolume(audio->musica, audio->volumeMusica);
    }

    salvarVolume(audio->volumeMusica);
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
