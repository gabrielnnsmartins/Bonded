#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "inimigo.h"

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
Texture2D texInimigo;

Jogador player;
float temporizadorFase = 0.0f;
const float TEMPO_ESPERA = 20.0f;
Rectangle rectBotaoStart; // Retângulo de colisão para o botão START


// --- Constantes de Animação ---
const int NUM_FRAMES = 6;
const int NUM_FRAMES_INIMIGOS = 4;
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
Rectangle GetPlayerRec();

int main() {
    // Inicialização da Janela
    const int larguraTela = 1280;
    const int alturaTela = 720;
    InitWindow(larguraTela, alturaTela, "Jogo da Cavaleira - Raylib Local");
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

Rectangle GetPlayerRec(){
    Rectangle rec =
    {
        player.posicao.x,
        player.posicao.y,
        (float)larguraFrame,
        (float)alturaFrame
    };
    return rec;
}

// --- Implementação das Funções ---

void InicializarJogo() {
    // --- CARREGAMENTO DAS SUAS IMAGENS ---
    // O código procura estes arquivos na mesma pasta onde o executável do jogo estiver.
    
    texTelaInicial = LoadTexture("scr/tela_de_inicio.jpg");
    
    // ATENÇÃO: Renomeie seu arquivo de sprite sheet para "personagem.png"
    texPersonagem = LoadTexture("scr/personagem.png"); 
    
    // Carregando os cenários conforme a numeração dos seus arquivos
    texFase1 = LoadTexture("scr/Fase1.jpg"); // Parede de tijolos
    texFase2 = LoadTexture("scr/Fase2.jpg"); // Campo verde
    texFase3 = LoadTexture("scr/Fase3.jpg"); //Piso quadriculado

    texInimigo = LoadTexture("scr/Inimigo.jpg"); // FALTA COLOCAR

    // Verificação de segurança: se a imagem da personagem não carregar, evita crash
    if (texPersonagem.id == 0) {
        TraceLog(LOG_WARNING, "ERRO: Nao foi possivel carregar 'personagem.png'. Verifique o nome do arquivo.");
         larguraFrame = 32; // Valores padrão para não travar
         alturaFrame = 32;
    } else {
        // Configurar Animação baseada no tamanho da imagem carregada
        larguraFrame = texPersonagem.width / NUM_FRAMES;
        alturaFrame = texPersonagem.height;
    }

    if (texInimigo.id == 0) {
        TraceLog(LOG_WARNING, "ERRO: Nao foi possivel carregar 'Inimigo.png'. Verifique o nome do arquivo.");
         larguraFrameInimigo= 32; // Valores padrão para não travar
         alturaFrameInimigo = 32;
    } else {
        // Configurar Animação baseada no tamanho da imagem carregada
        larguraFrameInimigo = texInimigo.width / NUM_FRAMES_INIMIGOS;
        alturaFrameInimigo= texInimigo.height;
    }


    // Definir a área do botão START na tela inicial
    // Ajustado para a posição do botão na sua imagem "tela_de_inicio.jpg"
    rectBotaoStart = (Rectangle){ 570, 180, 150, 60 };

    ResetarJogador();
}

void ResetarJogador() {
    player.posicao = (Vector2){ 400, 225 }; // Centro da tela
    player.velocidade = 200.0f;
    player.frameAtual = 0;
    player.tempoAnimacao = 0.0f;
    player.direcao = 1;
    player.movendo = false;
    // Reseta os aprendizados apenas se voltar para a tela inicial,
    // caso contrário, mantém para as fases seguintes.
    if (estadoAtual == TELA_INICIAL || estadoAtual == FASE_1) {
        player.moveuEsquerda = false;
        player.moveuDireita = false;
        player.moveuCima = false;
        player.moveuBaixo = false;
    }
}

void AtualizarJogo() {
    extern Inimigo *ListaInimigo;
    switch (estadoAtual) {
        case TELA_INICIAL:
            // Verificar clique no botão START com o mouse
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePos = GetMousePosition();
                if (CheckCollisionPointRec(mousePos, rectBotaoStart)) {
                    estadoAtual = FASE_1;
                    ResetarJogador();
                }
            }
            
            // Verificar pressionar a tecla ESPAÇO para iniciar o jogo
            if (IsKeyPressed(KEY_SPACE)) {
                estadoAtual = FASE_1;
                ResetarJogador();
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
                temporizadorFase = 0.0f;

                AdicionarInimigo((Vector2){1000, 500}, 150.0f, 100);
                AdicionarInimigo((Vector2){100, 50}, 180.0f, 100);

            }
            break;

        case FASE_2:
            // A fase 2 começa a contar o tempo imediatamente

            AtualizarJogador();
            AtualizarInimigos(player.posicao,GetFrameTime());
            VerificarColisao(GetPlayerRec());

            if (total_inimigos_vivos == 0){ //mudei ListaInimigo == NULL

                estadoAtual = ESPERA_F2;
                temporizadorFase = 0.0f;
                ResetarJogador();
    
                break;
            }

            temporizadorFase += GetFrameTime();
            if (temporizadorFase >= TEMPO_ESPERA) {
                estadoAtual = ESPERA_F2;
                ResetarJogador();
                
            }
            break;


        case ESPERA_F2:
            AtualizarJogador();

            temporizadorFase += GetFrameTime();
            if (temporizadorFase >= TEMPO_ESPERA) {
                estadoAtual = FASE_3;
                ResetarJogador();
                LiberarInimigos();
            }
            break; 

        case FASE_3:
            // A fase 3 começa a contar o tempo imediatamente
            estadoAtual = ESPERA_F3;
            temporizadorFase = 0.0f;
            //adcionar chefao ou obstaculo final;
            break;

        case ESPERA_F3:
            AtualizarJogador();
            temporizadorFase += GetFrameTime();
            if (temporizadorFase >= TEMPO_ESPERA) {
                estadoAtual = TELA_INICIAL; // Volta para o início
                ResetarJogador();
                LiberarInimigos();
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
        if (estadoAtual == FASE_1 || estadoAtual == ESPERA_F1) player.moveuDireita = true;
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        movimento.x -= 1.0f;
        player.direcao = -1;
        player.movendo = true;
        if (estadoAtual == FASE_1 || estadoAtual == ESPERA_F1) player.moveuEsquerda = true;
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        movimento.y += 1.0f;
        player.movendo = true;
        if (estadoAtual == FASE_1 || estadoAtual == ESPERA_F1) player.moveuBaixo = true;
    }
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        movimento.y -= 1.0f;
        player.movendo = true;
        if (estadoAtual == FASE_1 || estadoAtual == ESPERA_F1) player.moveuCima = true;
    }

    // Atualizar Posição
    player.posicao.x += movimento.x * player.velocidade * GetFrameTime();
    player.posicao.y += movimento.y * player.velocidade * GetFrameTime();

    // Manter o jogador dentro da tela
    if (texPersonagem.id != 0) { // Só tenta colidir se a textura existe
        if (player.posicao.x < 0) player.posicao.x = 0;
        if (player.posicao.y < 0) player.posicao.y = 0;
        if (player.posicao.x + larguraFrame > GetScreenWidth()) player.posicao.x = GetScreenWidth() - larguraFrame;
        if (player.posicao.y + alturaFrame > GetScreenHeight()) player.posicao.y = GetScreenHeight() - alturaFrame;
    }

    if (player.posicao.y < 0) player.posicao.y = 0;

    //RESTRIÇÃO DE 506 PX
    // Se a posição Y tentar ser maior que 506, força a ser 506.

    if (player.posicao.y < 105) {
        player.posicao.y = 105;
    }
 
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
    if (texPersonagem.id == 0) return; // Não desenha se a imagem falhou ao carregar

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
            if(texTelaInicial.id != 0) DrawTexture(texTelaInicial, 0, 0, WHITE);
            // Adicionar texto indicando que pode pressionar ESPAÇO
            DrawText("Pressione ESPACO ou clique em START para iniciar", 350, 650, 20, WHITE);
            break;

        case FASE_1:
            if(texFase1.id != 0) DrawTexture(texFase1, 0, 0, WHITE);
            DesenharJogador();
            DrawText("FASE 1: Use as setas (ou WASD) para se mover em todas as direcoes!", 10, 10, 20, WHITE);
            break;

        case ESPERA_F1:
            if(texFase1.id != 0) DrawTexture(texFase1, 0, 0, WHITE);
            DesenharJogador();
            DrawText(TextFormat("Muito bem! Aguarde: %.1f segundos...", TEMPO_ESPERA - temporizadorFase), 10, 10, 20, WHITE);
            break;

        case FASE_2:
        case ESPERA_F2:
            if(texFase2.id != 0) DrawTexture(texFase2, 0, 0, WHITE);
            DesenharJogador();
            DesenharInimigos();
            DrawText(TextFormat("FASE 2 (Campo) - Aguarde: %.1f segundos...", TEMPO_ESPERA - temporizadorFase), 10, 10, 20, BLACK);
            break;

        case FASE_3:
        case ESPERA_F3:
            if(texFase3.id != 0) DrawTexture(texFase3, 0, 0, WHITE);
            DrawText(TextFormat("FASE 3 (Piso) - Aguarde: %.1f segundos...", TEMPO_ESPERA - temporizadorFase), 10, 10, 20, BLACK);
            DesenharJogador();
            break;
    }

    // Se alguma textura falhou, avisa na tela
    if (texPersonagem.id == 0 || texTelaInicial.id == 0 || texFase1.id == 0) {
         DrawText("ERRO: Imagens nao encontradas na pasta do executavel!", 10, 400, 20, RED);
    }

    EndDrawing();
}

void DescarregarRecursos() {
    UnloadTexture(texTelaInicial);
    UnloadTexture(texPersonagem);
    UnloadTexture(texFase1);
    UnloadTexture(texFase2);
    UnloadTexture(texFase3);
    UnloadTexture(texInimigo);
    LiberarInimigos();
}

