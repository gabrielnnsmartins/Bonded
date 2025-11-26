#include "inimigo.h"
#include "jogador.h"
#include "raylib.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

Inimigo *ListaInimigo = NULL;
Texture2D texInimigo = {0};

int larguraFrameInimigo = 32;
int alturaFrameInimigo = 32;

static const int NUM_FRAMES = 8;
static const float FRAME_DELAY = 0.12f;
static const float INIMIGO_COOLDOWN = 0.7f;
static const int DANO_COLISAO = 10;

int total_inimigos_vivos = 0;

void CarregarInimigoTex(const char *path)
{
    texInimigo = LoadTexture(path);
    if (texInimigo.id == 0) {
        printf("Erro ao carregar textura de inimigo: %s\n", path);
        return;
    }

    larguraFrameInimigo = texInimigo.width / NUM_FRAMES;
    alturaFrameInimigo = texInimigo.height;
}

void UnloadTexInimigo() {
    if (texInimigo.id != 0)
        UnloadTexture(texInimigo);
}

void AdicionarInimigo(Vector2 pos, float velocidade, int vida)
{
    Inimigo *novo = malloc(sizeof(Inimigo));
    if (!novo) return;

    novo->pos = pos;
    novo->velocidade = velocidade;
    novo->vida = vida;   

    novo->frameAtual = 0;
    novo->tempoAnimacao = 0;
    novo->cooldown = INIMIGO_COOLDOWN;
    novo->ativo = true;

    novo->next = ListaInimigo;
    ListaInimigo = novo;

    total_inimigos_vivos++;
}

static void AtualizarUmInimigo(Inimigo *i, Vector2 jogador, float delta)
{

    i->cooldown += delta;

    Vector2 d = { jogador.x - i->pos.x, jogador.y - i->pos.y };
    float mag = sqrtf(d.x*d.x + d.y*d.y);

    if (mag > 0.1f) {
        d.x /= mag;
        d.y /= mag;
        i->pos.x += d.x * i->velocidade * delta;
        i->pos.y += d.y * i->velocidade * delta;
    }

    i->tempoAnimacao += delta;
    if (i->tempoAnimacao >= FRAME_DELAY) {
        i->tempoAnimacao = 0;
        i->frameAtual = (i->frameAtual + 1) % NUM_FRAMES;
    }
}

void AtualizarInimigos(Vector2 posJogador, float delta)
{
    Inimigo *atual = ListaInimigo;

    while (atual != NULL) {
        AtualizarUmInimigo(atual, posJogador, delta);
        atual = atual->next;
    }

    RemoverInimigosInativos();
}

void RemoverInimigosInativos()
{
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
        }
        else {
            ant = atual;
            atual = atual->next;
        }
    }
}

void VerificarColisao(Rectangle hitbox, Jogador *j)
{
    Inimigo *i = ListaInimigo;

    while (i != NULL) {
        Rectangle rIn = {
            i->pos.x,
            i->pos.y,
            larguraFrameInimigo,
            alturaFrameInimigo
        };

        if (CheckCollisionRecs(hitbox, rIn)) {
            if (i->cooldown >= INIMIGO_COOLDOWN)
            {
                j->hp -= DANO_COLISAO;
                if (j->hp < 0) j->hp = 0;
                i->cooldown = 0.0f;
            }
        }

        i = i->next;
    }
}

void VerificarColisaoAtaque(Rectangle hitbox, int dano)
{
    Inimigo *atual = ListaInimigo;

    while (atual != NULL) {

        Rectangle rIn = {
            atual->pos.x,
            atual->pos.y,
            larguraFrameInimigo,
            alturaFrameInimigo
        };

        if (CheckCollisionRecs(hitbox, rIn)) {
            atual->vida -= dano;
            return;
        }

        atual = atual->next;
    }
}

void DesenharInimigos()
{
    if (texInimigo.id == 0) return;

    Inimigo *atual = ListaInimigo;

    while (atual != NULL) {

        Rectangle src = {
            larguraFrameInimigo * atual->frameAtual,
            0,
            larguraFrameInimigo,
            alturaFrameInimigo
        };

        DrawTextureRec(texInimigo, src, atual->pos, WHITE);

        DrawText(TextFormat("%d", atual->vida),
                 atual->pos.x,
                 atual->pos.y - 12,
                 10, RED);

        atual = atual->next;
    }
}

void LiberarInimigos()
{
    Inimigo *atual = ListaInimigo;

    while (atual != NULL) {
        Inimigo *tmp = atual;
        atual = atual->next;
        free(tmp);
    }

    ListaInimigo = NULL;
    total_inimigos_vivos = 0;
}
