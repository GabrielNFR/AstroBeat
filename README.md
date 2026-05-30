# AstroBeat

## Descrição

**AstroBeat** é um jogo rítmico 3D de alta velocidade no estilo corrida de corredor infinito (endless runner).

O jogador pilota uma nave espacial através de uma pista neon intergaláctica dividida em três faixas. O objetivo principal é reagir aos obstáculos e às notas musicais que se aproximam na velocidade e no ritmo correto da música, alternando rapidamente entre as faixas esquerda, central e direita para desviar de perigos ou coletar notas musicais.

---

## Demonstração

[![Vídeo de demonstração do AstroBeat](https://img.youtube.com/vi/OBFhsgefYEo/hqdefault.jpg)](https://youtu.be/OBFhsgefYEo)

Assista ao vídeo de demonstração executando o jogo: https://youtu.be/OBFhsgefYEo

---

## Instalação, compilação e execução

### Pré-requisitos

O jogo foi desenvolvido em C utilizando a biblioteca gráfica **raylib**. Para compilar e executar o projeto em Linux, é necessário ter instalado:

- `gcc`
- `raylib`
- bibliotecas gráficas e de sistema usadas pela raylib no Linux
- `pkg-config`, opcional, mas recomendado

Em distribuições baseadas em Debian/Ubuntu, tente instalar com:

```bash
sudo apt update
sudo apt install build-essential pkg-config libraylib-dev
```

Caso o pacote `libraylib-dev` não esteja disponível na sua distribuição, instale a raylib manualmente seguindo a documentação oficial:

https://github.com/raysan5/raylib

Em Arch Linux:

```bash
sudo pacman -S base-devel raylib
```

Em Fedora:

```bash
sudo dnf install gcc pkg-config raylib-devel
```

Para verificar se a raylib foi instalada corretamente:

```bash
pkg-config --modversion raylib
```

Se o comando acima mostrar uma versão da raylib, a biblioteca está disponível para compilação.

---

### Clonando o repositório

```bash
git clone https://github.com/GabrielNFR/AstroBeat.git
cd AstroBeat
```

É importante executar os comandos a partir da raiz do projeto, pois o jogo carrega arquivos usando caminhos relativos, como:

- `assets/`
- `songs/`
- `mapasMusicas/`
- `mapasColetaveis/`

---

### Compilando o jogo

A forma recomendada de compilar é usando `pkg-config`:

```bash
gcc src/*.c -o astrobeat $(pkg-config --cflags --libs raylib) -lm
```

Caso `pkg-config` não esteja disponível, também é possível compilar informando manualmente as bibliotecas necessárias:

```bash
gcc src/*.c -o astrobeat -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

Esse comando gera o executável chamado `astrobeat` na raiz do projeto.

---

### Executando

Após compilar, execute:

```bash
./astrobeat
```

O jogo deve ser executado a partir da raiz do repositório para que os arquivos de assets, músicas e mapas sejam encontrados corretamente.

---

### Arquivos gerados durante a execução

Durante o uso do jogo, alguns arquivos de configuração podem ser criados ou atualizados automaticamente:

- `configuracoes.cfg`: salva configurações como volume.
- `highscores.cfg`: salva as maiores pontuações.

Esses arquivos não são necessários para compilar o projeto; eles são usados apenas em tempo de execução.

---

### Observação sobre Makefile

Este projeto não utiliza Makefile. A compilação é feita diretamente pela linha de comando com `gcc`, conforme mostrado acima. Portanto, para fins de avaliação, a instrução de compilação do projeto é:

```bash
gcc src/*.c -o astrobeat -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```
---

## Créditos 

Um agradecimento especial ao criador do modelo da nave utilizado neste projeto:

* **Modelo da Nave (Voxel):** [Voxel Spaceships](https://maxparata.itch.io/voxel-spaceships) por **Max Parata**.
