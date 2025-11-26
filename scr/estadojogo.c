#include "estadojogo.h"
#include "jogador.h"
#include "inimigo.h"
#include "leaderboard.h"
#include <stdio.h>

#define TEMPO_LIMITE_PADRAO 120.0f

extern Jogador j;
extern Inimigo *ListaInimigo;
extern Texture2D texInimigo;
extern int total_inimigos_vivos;

estadojogo estado_atual = TELA_INICIAL;

Texture2D imgTELA_INICIAL = {0};
Texture2D imgFase[6] = {0};

int faseAtual = 0;

float tempoTotalPartida = 0.0f; 
float tempoRestante = -1.0f;  

typedef struct {
    int inimigos;
    float velocidade;
    int vida;
    float tempoLimite; 
} parametroFase;

parametroFase parametroFases[6] = {
    {0,    0.0f, 0, 0.0f},   
    {5,    3.0f, 10, 120.0f}, 
    {8,    3.5f, 10, 150.0f}, 
    {10,   4.0f, 20, 180.0f}, 
    {12,   4.5f, 20, 200.0f}, 
    {15,   5.0f, 30, 240.0f} 
};

void Estado_Iniciar(void) {
    
    imgTELA_INICIAL = LoadTexture("assets/tela_de_inicio.jpg");
    imgFase[1] = LoadTexture("assets/Fase1.jpg");
    imgFase[2] = LoadTexture("assets/Fase2.jpg");
    imgFase[3] = LoadTexture("assets/Fase3.jpg");
    imgFase[4] = LoadTexture("assets/Fase4.jpg");
    imgFase[5] = LoadTexture("assets/Fase5.jpg");

    CarregarInimigoTex("assets/Inimigo.png");

    faseAtual = 0;
    tempoTotalPartida = 0.0f;
    tempoRestante = -1.0f;
    estado_atual = TELA_INICIAL;
}

void Estado_Finalizar(void) {

    if (imgTELA_INICIAL.id != 0) UnloadTexture(imgTELA_INICIAL);
    for (int i = 1; i <= 5; i++) {
        if (imgFase[i].id != 0) UnloadTexture(imgFase[i]);
    }

    UnloadTexInimigo();

    LiberarInimigos();
}

static void Estado_ComecoFase(int fase) {

    LiberarInimigos();

    faseAtual = fase;
    
    estado_atual = (estadojogo)(FASE_1 + (fase - 1));

    tempoTotalPartida = 0.0f;
    float limite = parametroFases[fase].tempoLimite;
    if (limite > 0.0f) tempoRestante = limite;
    else tempoRestante = -1.0f; 

    partidaEmAndamento = true;

    j.x = 100;
    j.y = 100;
    j.hp = 100;
    j.hitbox.x = (float)j.x;
    j.hitbox.y = (float)j.y;

    parametroFase p = parametroFases[fase];
    for (int i = 0; i < p.inimigos; i++) {
        float px = (float)GetRandomValue(80, 1180);
        float py = (float)GetRandomValue(80, 620);
        AdicionarInimigo((Vector2){px, py}, p.velocidade, p.vida);
    }
}

void Estado_Desenhar(void) {
    char buf[64];

    switch (estado_atual) {
        case TELA_INICIAL:
            if (imgTELA_INICIAL.id != 0) DrawTexture(imgTELA_INICIAL, 0, 0, WHITE);
            else ClearBackground(RAYWHITE);

            DrawText("BONDED MVP - Pressione ESPACO para iniciar",350, 650, 20, WHITE);
            DrawText("WASD para mover, clique esquerdo para atacar", 350, 86, 20, WHITE);
            DrawText("5 fases. Derrote todos os inimigos para avançar.", 350, 120, 20, DARKGRAY);
            break;

        case FASE_1:
        case FASE_2:
        case FASE_3:
        case FASE_4:
        case FASE_FINAL:

            if (faseAtual >= 1 && faseAtual <= 5 && imgFase[faseAtual].id != 0) {
                DrawTexture(imgFase[faseAtual], 0, 0, WHITE);
            } else {
                ClearBackground(RAYWHITE);
            }

            desenhar_jogador(&j);
            DesenharInimigos();

            sprintf(buf, "Fase: %d", faseAtual);
            DrawText(buf, 10, 10, 20, BLACK);

            sprintf(buf, "HP: %d", j.hp);
            DrawText(buf, 10, 36, 20, RED);

            sprintf(buf, "Tempo (decorrido): %.2f s", tempoTotalPartida);
            DrawText(buf, 10, 62, 18, BLACK);

            if (tempoRestante > 0.0f) {
                sprintf(buf, "Tempo restante: %.0f s", tempoRestante);
                DrawText(buf, 10, 86, 18, BLACK);
            }

            sprintf(buf, "Inimigos vivos: %d", total_inimigos_vivos);
            DrawText(buf, 10, 110, 18, BLACK);

            break;

        case TELA_GANHOU:
            ClearBackground(RAYWHITE);
            DrawText("PARABENS! VOCE VENCEU O JOGO!", 120, 200, 30, GREEN);
            DrawText("Pressione ESPACO para voltar ao menu", 120, 240, 20, DARKGRAY);
            DesenharLeaderboard(200, 320);
            break;

        case TELA_PERDEU:
            ClearBackground(RAYWHITE);
            DrawText("VOCE PERDEU! Pressione ESPACO para tentar novamente", 120, 200, 30, RED);
            DrawText("Pressione ESPACO para voltar ao menu", 120, 240, 20, DARKGRAY);
            DesenharLeaderboard(200, 320);
            break;

        default:
            ClearBackground(RAYWHITE);
            DrawText("Estado Desconhecido", 200, 200, 20, BLACK);
            break;
    }
}

void Estado_Update(void) {
    float delta = GetFrameTime();

    if (partidaEmAndamento) {
        tempoTotalPartida += delta;
        if (tempoRestante > 0.0f) {
            tempoRestante -= delta;
            if (tempoRestante <= 0.0f) {
                estado_atual = TELA_PERDEU;
                partidaEmAndamento = false;
                LiberarInimigos();
                return;
            }
        }
    }

    if (estado_atual == FASE_1 || estado_atual == FASE_2 || estado_atual == FASE_3 ||
        estado_atual == FASE_4 || estado_atual == FASE_FINAL) {

        mover_jogador(&j);
        atualizar_jogador(&j);

        Vector2 posJogador = { (float)j.x, (float)j.y };
        AtualizarInimigos(posJogador, delta);
        VerificarColisao(j.hitbox, &j);

        if (j.atacando && !j.ataqueprocessado) {
            Rectangle hitboxAtaque = {
                (float)j.x + (j.ultimadirecaoH == 1 ? j.framelargura : -j.larguraframeataque),
                (float)j.y,
                (float)j.larguraframeataque,
                (float)j.alturaframeataque
            };
            VerificarColisaoAtaque(hitboxAtaque, 1); 
            j.ataqueprocessado = true;
        }
    }

    if (j.hp <= 0) {
        estado_atual = TELA_PERDEU;
        partidaEmAndamento = false;
        LiberarInimigos();
        return;
    }

    if (faseAtual >= 1 && faseAtual <= 5 && total_inimigos_vivos <= 0 &&
        (estado_atual == FASE_1 || estado_atual == FASE_2 || estado_atual == FASE_3 ||
         estado_atual == FASE_4 || estado_atual == FASE_FINAL)) {

        partidaEmAndamento = false;

        if (faseAtual < 5) {
            Estado_ComecoFase(faseAtual + 1);
            return;
        } else {
            estado_atual = TELA_GANHOU;
            ultimoTempo = tempoTotalPartida;
            SalvarLeaderboard(ultimoTempo);
            LiberarInimigos();
            return;
        }
    }

    switch (estado_atual) {
        case TELA_INICIAL:
            if (IsKeyPressed(KEY_SPACE)) {
                Estado_ComecoFase(1);
            }
            break;

        case TELA_PERDEU:
            if (IsKeyPressed(KEY_SPACE)) {
                LiberarInimigos();
                estado_atual = TELA_INICIAL;
            }
            break;

        case TELA_GANHOU:
            if (IsKeyPressed(KEY_SPACE)) {
                LiberarInimigos();
                estado_atual = TELA_INICIAL;
            }
            break;

        default:
            break;
    }
}
