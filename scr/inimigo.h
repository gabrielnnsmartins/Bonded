#ifndef INIMIGO_H
#define INIMIGO_H

#include "raylib.h"


typedef struct NoInimigo {
    Vector2 posicao;
    float velocidade;
    int frameAtual;
    float tempoAnimacao;
    int vida;
    bool ativo;
    struct NoInimigo *next;
} Inimigo;

extern Inimigo *ListaInimigo;
extern Texture2D texInimigo;
extern int larguraFrameInimigo;
extern int alturaFrameInimigo;

extern int total_inimigos_vivos;


void AdicionarInimigo(Vector2 posInicial, float velocidade, int vida);
void AtualizarInimigos(Vector2 posJogador, float delta);
void RemoverInimigosInativos();
void LiberarInimigos();
void DesenharInimigos();
void VerificarColisao(Rectangle player);
void VerificarColisaoAtaque(Rectangle hitbox, int dano);
#endif
