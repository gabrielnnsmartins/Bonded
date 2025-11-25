#include "inimigo.h"
#include <stdlib.h>
#include <math.h>

Inimigo *ListaInimigo = NULL;
int total_inimigos_vivos = 0;

int larguraFrameInimigo = 32;
int alturaFrameInimigo = 32;

//Cria um inimigo
void AdicionarInimigo(Vector2 posInicial, float velocidade, int vida) {
    Inimigo *novo = malloc(sizeof(Inimigo));
    if (!novo){
        return;
    }
    novo->posicao = posInicial;
    novo->velocidade = velocidade;
    novo->vida = vida;
    novo->frameAtual = 0;
    novo->tempoAnimacao = 0;
    novo->next = ListaInimigo;
    ListaInimigo = novo;
    total_inimigos_vivos++;
}


//  Atualizar movimento e animação

void AtualizarInimigos(Vector2 posJogador, float delta) {
    Inimigo *atual = ListaInimigo;

    while (atual != NULL) {

        Vector2 d = { posJogador.x - atual->posicao.x, posJogador.y - atual->posicao.y };
        float mag = sqrtf(d.x*d.x + d.y*d.y);

        if (mag > 1.0f) {
            d.x /= mag;
            d.y /= mag;
        }

        atual->posicao.x += d.x * atual->velocidade * delta;
        atual->posicao.y += d.y * atual->velocidade * delta;

        atual->tempoAnimacao += delta;
        if (atual->tempoAnimacao > 0.1f) {
            atual->tempoAnimacao = 0;
            atual->frameAtual = (atual->frameAtual + 1) % 4;
        }

        atual = atual->next;
    }

    RemoverInimigosInativos();
}

//  Remoção dos mortos

void RemoverInimigosInativos() {
    Inimigo *atual = ListaInimigo;
    Inimigo *ant = NULL;

    while (atual != NULL) {
        if (atual->vida <= 0) {

            total_inimigos_vivos--;

            if (ant == NULL) {
                ListaInimigo = atual->next;
                free(atual);
                atual = ListaInimigo;
            } else {
                ant->next = atual->next;
                free(atual);
                atual = ant->next;
            }

        } else {
            ant = atual;
            atual = atual->next;
        }
    }
}


//  Desenhar os inimigos

void DesenharInimigos() {
    extern Texture2D texInimigo;
    Inimigo *atual = ListaInimigo;

    if (texInimigo.id == 0){
        return;
    }
    while (atual != NULL) {
        Rectangle src = {
            atual->frameAtual * larguraFrameInimigo,
            0,
            larguraFrameInimigo,
            alturaFrameInimigo
        };

        DrawTextureRec(texInimigo, src, atual->posicao, WHITE);

        DrawText(TextFormat("%d", atual->vida),
                 atual->posicao.x,
                 atual->posicao.y - 15,
                 10,
                 RED);

        atual = atual->next;
    }
}


//  Colisão entre inimigo e jogador

void VerificarColisao(Rectangle player) {
    Inimigo *atual = ListaInimigo;

    while (atual != NULL) {
        Rectangle rIn = {
            atual->posicao.x,
            atual->posicao.y,
            larguraFrameInimigo,
            alturaFrameInimigo
        };

        if (CheckCollisionRecs(player, rIn)) {
            atual->vida = 0;
            return;
        }

        atual = atual->next;
    }
}


//  Liberar tudo da memória

void LiberarInimigos() {
    Inimigo *atual = ListaInimigo;

    while (atual != NULL) {
        Inimigo *tmp = atual;
        atual = atual->next;
        free(tmp);
    }

    ListaInimigo = NULL;
    total_inimigos_vivos =0;
}
