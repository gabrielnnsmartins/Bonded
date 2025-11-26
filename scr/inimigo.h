#ifndef INIMIGO_H
#define INIMIGO_H

#include "raylib.h"
#include "jogador.h"
#include <stdbool.h>

typedef struct Inimigo {
    Vector2 pos;
    int vida;
    float velocidade;

    int frameAtual;
    float tempoAnimacao;

    bool ativo;
    float cooldown;
    struct Inimigo *next;
} Inimigo;

extern Inimigo *ListaInimigo;
extern Texture2D texInimigo;
extern int larguraFrameInimigo;
extern int alturaFrameInimigo;

extern int total_inimigos_vivos;

void CarregarInimigoTex(const char *path);
void UnloadTexInimigo();

void AdicionarInimigo(Vector2 posInicial, float velocidade, int vida);
void AtualizarInimigos(Vector2 posJogador, float delta);
void RemoverInimigosInativos();
void DesenharInimigos();
void LiberarInimigos();

void VerificarColisao(Rectangle hitbox, Jogador *j);
void VerificarColisaoAtaque(Rectangle hitbox, int dano);

#endif
