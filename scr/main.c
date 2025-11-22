#include "raylib.h"
#include <stdio.h>

// --- Definições de Estados do Jogo ---
typedef enum {
    TELA_INICIAL,
    FASE_1,
    ESPERA_F1,
    FASE_2,
    ESPERA_F2,
    FASE_3,
    ESPERA_F3
} EstadoJogo;

// --- Estrutura do Jogador ---
typedef struct {
    Vector2 posicao;
    float velocidade;
    int frameAtual;
    float tempoAnimacao;
    int direcao; // 1 = Direita, -1 = Esquerda
    bool movendo;
    // Flags para rastrear se o jogador aprendeu a se mover
    bool moveuEsquerda;
    bool moveuDireita;
    bool moveuCima;
    bool moveuBaixo;
} Jogador;

// --- Variáveis Globais ---
EstadoJogo estadoAtual = TELA_INICIAL;
Texture2D texTelaInicial, texPersonagem, texFase1, texFase2, texFase3;
Jogador player;
float temporizadorFase = 0.0f;
const float TEMPO_ESPERA = 20.0f;
Rectangle rectBotaoStart; // Retângulo de colisão para o botão START

// --- Constantes de Animação ---
const int NUM_FRAMES = 6;
const float VELOCIDADE_ANIMACAO = 0.1f; // Tempo por frame
int larguraFrame;
int alturaFrame;

// --- Protótipos de Funções ---
void InicializarJogo();
void AtualizarJogo();
void DesenharJogo();
void DescarregarRecursos();
void AtualizarJogador();
void DesenharJogador();
void ResetarJogador();

int main() {
    // Inicialização da Janela
    const int larguraTela = 800;
    const int alturaTela = 450;
    InitWindow(larguraTela, alturaTela, "Jogo da Cavaleira - Raylib");
    SetTargetFPS(60);

    InicializarJogo();

    // Loop Principal do Jogo
    while (!WindowShouldClose()) {
        AtualizarJogo();
        DesenharJogo();
    }

    DescarregarRecursos();
    CloseWindow();

    return 0;
}

// --- Implementação das Funções ---

void InicializarJogo() {
    // Carregar Texturas
    texTelaInicial = LoadTexture("image_3.png");
    texPersonagem = LoadTexture("image_1.png");
    texFase1 = LoadTexture("image_4.png"); // Cenário externo com castelo
    texFase2 = LoadTexture("image_0.png"); // Piso quadriculado
    texFase3 = LoadTexture("image_2.png"); // Parede de tijolos

    // Configurar Animação
    larguraFrame = texPersonagem.width / NUM_FRAMES;
    alturaFrame = texPersonagem.height;

    // Definir a área do botão START na tela inicial (valores aproximados, ajuste se necessário)
    rectBotaoStart = (Rectangle){ 570, 180, 150, 50 };

    ResetarJogador();
}

void ResetarJogador() {
    player.posicao = (Vector2){ 400, 225 }; // Centro da tela
    player.velocidade = 200.0f;
    player.frameAtual = 0;
    player.tempoAnimacao = 0.0f;
    player.direcao = 1;
    player.movendo = false;
    player.moveuEsquerda = false;
    player.moveuDireita = false;
    player.moveuCima = false;
    player.moveuBaixo = false;
}

void AtualizarJogo() {
    switch (estadoAtual) {
        case TELA_INICIAL:
            // Verificar clique no botão START
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePos = GetMousePosition();
                if (CheckCollisionPointRec(mousePos, rectBotaoStart)) {
                    estadoAtual = FASE_1;
                    ResetarJogador();
                }
            }
            break;

        case FASE_1:
            AtualizarJogador();
            // Verificar se todas as direções foram usadas
            if (player.moveuEsquerda && player.moveuDireita && player.moveuCima && player.moveuBaixo) {
                estadoAtual = ESPERA_F1;
                temporizadorFase = 0.0f;
            }
            break;

        case ESPERA_F1:
            AtualizarJogador(); // O jogador ainda pode se mover
            temporizadorFase += GetFrameTime();
            if (temporizadorFase >= TEMPO_ESPERA) {
                estadoAtual = FASE_2;
                ResetarJogador();
            }
            break;

        case FASE_2:
            // A fase 2 começa a contar o tempo imediatamente
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
            // A fase 3 começa a contar o tempo imediatamente
            estadoAtual = ESPERA_F3;
            temporizadorFase = 0.0f;
            break;

        case ESPERA_F3:
            AtualizarJogador();
            temporizadorFase += GetFrameTime();
            if (temporizadorFase >= TEMPO_ESPERA) {
                estadoAtual = TELA_INICIAL; // Volta para o início
            }
            break;
    }
}

void AtualizarJogador() {
    player.movendo = false;
    Vector2 movimento = { 0.0f, 0.0f };

    // Controles de Movimento
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        movimento.x += 1.0f;
        player.direcao = 1;
        player.movendo = true;
        if (estadoAtual == FASE_1) player.moveuDireita = true;
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        movimento.x -= 1.0f;
        player.direcao = -1;
        player.movendo = true;
        if (estadoAtual == FASE_1) player.moveuEsquerda = true;
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        movimento.y += 1.0f;
        player.movendo = true;
        if (estadoAtual == FASE_1) player.moveuBaixo = true;
    }
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        movimento.y -= 1.0f;
        player.movendo = true;
        if (estadoAtual == FASE_1) player.moveuCima = true;
    }

    // Atualizar Posição
    player.posicao.x += movimento.x * player.velocidade * GetFrameTime();
    player.posicao.y += movimento.y * player.velocidade * GetFrameTime();

    // Manter o jogador dentro da tela
    if (player.posicao.x < 0) player.posicao.x = 0;
    if (player.posicao.y < 0) player.posicao.y = 0;
    if (player.posicao.x + larguraFrame > GetScreenWidth()) player.posicao.x = GetScreenWidth() - larguraFrame;
    if (player.posicao.y + alturaFrame > GetScreenHeight()) player.posicao.y = GetScreenHeight() - alturaFrame;


    // Lógica de Animação
    player.tempoAnimacao += GetFrameTime();
    if (player.tempoAnimacao >= VELOCIDADE_ANIMACAO) {
        player.tempoAnimacao = 0.0f;
        if (player.movendo) {
            player.frameAtual++;
        }
    }
}

void DesenharJogador() {
    Rectangle sourceRec;
    int frameIndex = 0;

    if (player.direcao == -1) { // Esquerda
        if (player.movendo) {
            frameIndex = player.frameAtual % 3; // Frames 0, 1, 2
        } else {
            frameIndex = 0; // Frame parado
        }
    } else { // Direita
        if (player.movendo) {
            // Frames 5, 4, 3 (ordem inversa)
            frameIndex = 5 - (player.frameAtual % 3);
        } else {
            frameIndex = 5; // Frame parado
        }
    }

    sourceRec = (Rectangle){ frameIndex * larguraFrame, 0, larguraFrame, alturaFrame };
    DrawTextureRec(texPersonagem, sourceRec, player.posicao, WHITE);
}

void DesenharJogo() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    switch (estadoAtual) {
        case TELA_INICIAL:
            DrawTexture(texTelaInicial, 0, 0, WHITE);
            // Descomente a linha abaixo para ver o retângulo do botão START para depuração
            // DrawRectangleRec(rectBotaoStart, (Color){255, 0, 0, 100}); 
            break;

        case FASE_1:
            DrawTexture(texFase1, 0, 0, WHITE);
            DesenharJogador();
            DrawText("FASE 1: Use as setas para se mover em todas as direções!", 10, 10, 20, BLACK);
            break;

        case ESPERA_F1:
            DrawTexture(texFase1, 0, 0, WHITE);
            DesenharJogador();
            DrawText(TextFormat("Aguarde: %.1f segundos...", TEMPO_ESPERA - temporizadorFase), 10, 10, 20, BLACK);
            break;

        case FASE_2:
        case ESPERA_F2:
            DrawTexture(texFase2, 0, 0, WHITE);
            DesenharJogador();
            DrawText(TextFormat("FASE 2 - Aguarde: %.1f segundos...", TEMPO_ESPERA - temporizadorFase), 10, 10, 20, WHITE);
            break;

        case FASE_3:
        case ESPERA_F3:
            DrawTexture(texFase3, 0, 0, WHITE);
            DesenharJogador();
            DrawText(TextFormat("FASE 3 - Aguarde: %.1f segundos...", TEMPO_ESPERA - temporizadorFase), 10, 10, 20, WHITE);
            break;
    }

    EndDrawing();
}

void DescarregarRecursos() {
    UnloadTexture(texTelaInicial);
    UnloadTexture(texPersonagem);
    UnloadTexture(texFase1);
    UnloadTexture(texFase2);
    UnloadTexture(texFase3);
}