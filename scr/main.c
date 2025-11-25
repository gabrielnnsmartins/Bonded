#include "raylib.h"
#include <stdio.h>

typedef enum {
    TELA_INICIAL,
    FASE_1,
    ESPERA_F1,
    FASE_2,
    ESPERA_F2,
    FASE_3,
    ESPERA_F3
} EstadoJogo;

typedef struct {
    Vector2 posicao;
    float velocidade;
    int frameAtual;
    float tempoAnimacao;
    int direcao;
    bool movendo;
    bool moveuEsquerda;
    bool moveuDireita;
    bool moveuCima;
    bool moveuBaixo;
    bool atacando;
    float tempoAtaque;
    int ultimaDirecaoHorizontal;
} Jogador;

typedef struct {
    Vector2 posicao;
    int vida;
    bool ativo;
} Inimigo;

EstadoJogo estadoAtual = TELA_INICIAL;
Texture2D texTelaInicial, texPersonagem, texAtaque, texFase1, texFase2, texFase3;
Jogador player;
float temporizadorFase = 0.0f;
const float TEMPO_ESPERA = 20.0f;
Rectangle rectBotaoStart;

const int NUM_FRAMES = 6;
const float VELOCIDADE_ANIMACAO = 0.1f;
int larguraFrame;
int alturaFrame;

const float DURACAO_ATAQUE = 0.12f;
const int DANO_ATAQUE = 50;
const float ALCANCE_ATAQUE = 80.0f;
int larguraFrameAtaque = 120;
int alturaFrameAtaque;

void InicializarJogo();
void AtualizarJogo();
void DesenharJogo();
void DescarregarRecursos();
void AtualizarJogador();
void DesenharJogador();
void ResetarJogador();
void ExecutarAtaque();
Rectangle ObterHitboxAtaque();

int main() {
    const int larguraTela = 1280;
    const int alturaTela = 720;
    InitWindow(larguraTela, alturaTela, "Jogo da Cavaleira - Raylib Local");
    SetTargetFPS(60);

    InicializarJogo();

    while (!WindowShouldClose()) {
        AtualizarJogo();
        DesenharJogo();
    }

    DescarregarRecursos();
    CloseWindow();
    return 0;
}

void InicializarJogo() {
    texTelaInicial = LoadTexture("tela_de_inicio.jpg");
    texPersonagem = LoadTexture("personagem.png");
    texAtaque = LoadTexture("ataque.png");
    texFase1 = LoadTexture("Fase1.jpg");
    texFase2 = LoadTexture("Fase2.jpg");
    texFase3 = LoadTexture("Fase3.jpg");

    larguraFrame = texPersonagem.width / NUM_FRAMES;
    alturaFrame = texPersonagem.height;
    alturaFrameAtaque = texAtaque.height;

    rectBotaoStart = (Rectangle){ 570, 180, 150, 60 };
    ResetarJogador();
}

void ResetarJogador() {
    player.posicao = (Vector2){ 400, 225 };
    player.velocidade = 200.0f;
    player.frameAtual = 0;
    player.tempoAnimacao = 0.0f;
    player.direcao = 1;
    player.movendo = false;
    player.atacando = false;
    player.tempoAtaque = 0.0f;
    player.ultimaDirecaoHorizontal = 1;

    player.moveuEsquerda = false;
    player.moveuDireita = false;
    player.moveuCima = false;
    player.moveuBaixo = false;
}

void AtualizarJogo() {
    switch (estadoAtual) {
        case TELA_INICIAL:
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePos = GetMousePosition();
                if (CheckCollisionPointRec(mousePos, rectBotaoStart)) {
                    estadoAtual = FASE_1;
                    ResetarJogador();
                }
            }
            if (IsKeyPressed(KEY_SPACE)) {
                estadoAtual = FASE_1;
                ResetarJogador();
            }
            break;

        case FASE_1:
            AtualizarJogador();
            if (player.moveuEsquerda && player.moveuDireita && player.moveuCima && player.moveuBaixo) {
                estadoAtual = ESPERA_F1;
                temporizadorFase = 0.0f;
            }
            break;

        case ESPERA_F1:
            AtualizarJogador();
            temporizadorFase += GetFrameTime();
            if (temporizadorFase >= TEMPO_ESPERA) {
                estadoAtual = FASE_2;
                ResetarJogador();
            }
            break;

        case FASE_2:
            estadoAtual = ESPERA_F2;
            temporizadorFase = 0.0f;
            break;

        case ESPERA_F2:
            AtualizarJogador();
            temporizadorFase += GetFrameTime();
            if (temporizadorFase >= TEMPO_ESPERA) {
                estadoAtual = FASE_3;
                ResetarJogador();
            }
            break;

        case FASE_3:
            estadoAtual = ESPERA_F3;
            temporizadorFase = 0.0f;
            break;

        case ESPERA_F3:
            AtualizarJogador();
            temporizadorFase += GetFrameTime();
            if (temporizadorFase >= TEMPO_ESPERA) {
                estadoAtual = TELA_INICIAL;
                ResetarJogador();
            }
            break;
    }
}

void AtualizarJogador() {
    if (player.atacando) {
        player.tempoAtaque += GetFrameTime();
        if (player.tempoAtaque >= DURACAO_ATAQUE) {
            player.atacando = false;
            player.tempoAtaque = 0.0f;
        }
        return;
    }

    if (IsKeyPressed(KEY_F)) {
        ExecutarAtaque();
        return;
    }

    player.movendo = false;
    Vector2 movimento = {0};

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        movimento.x += 1.0f;
        player.direcao = 1;
        player.ultimaDirecaoHorizontal = 1;
        player.movendo = true;
        player.moveuDireita = true;
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        movimento.x -= 1.0f;
        player.direcao = -1;
        player.ultimaDirecaoHorizontal = -1;
        player.movendo = true;
        player.moveuEsquerda = true;
    }
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        movimento.y -= 1.0f;
        player.direcao = player.ultimaDirecaoHorizontal;
        player.movendo = true;
        player.moveuCima = true;
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        movimento.y += 1.0f;
        player.direcao = player.ultimaDirecaoHorizontal;
        player.movendo = true;
        player.moveuBaixo = true;
    }

    player.posicao.x += movimento.x * player.velocidade * GetFrameTime();
    player.posicao.y += movimento.y * player.velocidade * GetFrameTime();

    if (player.posicao.y < 105) player.posicao.y = 105;

    player.tempoAnimacao += GetFrameTime();
    if (player.tempoAnimacao >= VELOCIDADE_ANIMACAO) {
        player.tempoAnimacao = 0.0f;
        if (player.movendo) player.frameAtual++;
    }
}

void ExecutarAtaque() {
    player.atacando = true;
    player.tempoAtaque = 0.0f;
}

Rectangle ObterHitboxAtaque() {
    Rectangle hitbox = {0};
    if (player.ultimaDirecaoHorizontal == 1) {
        hitbox.x = player.posicao.x + larguraFrame;
        hitbox.width = ALCANCE_ATAQUE;
    } else {
        hitbox.x = player.posicao.x - ALCANCE_ATAQUE;
        hitbox.width = ALCANCE_ATAQUE;
    }
    hitbox.y = player.posicao.y;
    hitbox.height = alturaFrame;
    return hitbox;
}

void DesenharJogador() {
    if (player.atacando) {
        int frameIndex = (player.ultimaDirecaoHorizontal == 1 ? 1 : 0);
        Rectangle sourceRec = (Rectangle){ frameIndex * larguraFrameAtaque, 0, larguraFrameAtaque, alturaFrameAtaque };
        Vector2 posicaoAtaque = {
            player.posicao.x - (larguraFrameAtaque - larguraFrame) / 2,
            player.posicao.y - (alturaFrameAtaque - alturaFrame) / 2
        };
        DrawTextureRec(texAtaque, sourceRec, posicaoAtaque, WHITE);
        return;
    }

    int frameIndex = 0;
    if (player.ultimaDirecaoHorizontal == -1) {
        frameIndex = player.movendo ? (player.frameAtual % 3) : 0;
    } else {
        frameIndex = player.movendo ? 5 - (player.frameAtual % 3) : 5;
    }

    Rectangle sourceRec = (Rectangle){ frameIndex * larguraFrame, 0, larguraFrame, alturaFrame };
    DrawTextureRec(texPersonagem, sourceRec, player.posicao, WHITE);
}

void DesenharJogo() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    switch (estadoAtual) {
        case TELA_INICIAL:
            if (texTelaInicial.id != 0) DrawTexture(texTelaInicial, 0, 0, WHITE);
            DrawText("Pressione ESPACO ou clique em START para iniciar", 350, 650, 20, WHITE);
            break;

        case FASE_1:
            DrawTexture(texFase1, 0, 0, WHITE);
            DesenharJogador();
            break;

        case ESPERA_F1:
            DrawTexture(texFase1, 0, 0, WHITE);
            DesenharJogador();
            break;

        case FASE_2:
        case ESPERA_F2:
            DrawTexture(texFase2, 0, 0, WHITE);
            DesenharJogador();
            break;

        case FASE_3:
        case ESPERA_F3:
            DrawTexture(texFase3, 0, 0, WHITE);
            DesenharJogador();
            break;
    }

    EndDrawing();
}

void DescarregarRecursos() {
    UnloadTexture(texTelaInicial);
    UnloadTexture(texPersonagem);
    UnloadTexture(texAtaque);
    UnloadTexture(texFase1);
    UnloadTexture(texFase2);
    UnloadTexture(texFase3);
}
