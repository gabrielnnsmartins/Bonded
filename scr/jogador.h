// cria os structs e chama as funções do jogador.c

#ifndef jogador_h
#define jogador_h
#include <raylib.h>
#include <stdbool.h>

typedef struct {
    int x, y;
    int hp;
    Texture2D sprite;
    const char *spritePath;
    Rectangle hitbox;

    //para a animação do sprite dar certo
    int frameatual;
    int numframes;
    int framelargura;
    float tempoanimacao;
    float framedelay;

    bool movendo;
    int ultimadirecaoH;

    //para o ataque
    bool atacando;
    float tempoataque;
    float duracaoataque;
    Texture2D spriteataque;
    int larguraframeataque;
    int alturaframeataque;

} Jogador;

void iniciar_jogador(Jogador *j, const char *spritePath);
void atualizar_jogador(Jogador *j);
void mover_jogador(Jogador *j);
void desenhar_jogador (const Jogador *j);
void unload_jogador(Jogador *j);
void atacar(Jogador *j);

#endif