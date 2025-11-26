#include "estadojogo.h"
#include "jogador.h"
#include "inimigo.h"
#include "leaderboard.h"
#include <stdio.h>

#define TEMPO_LIMITE 120

extern Jogador j;
extern Inimigo *ListaInimigo;
extern Texture2D texInimigo;
extern int total_inimigos_vivos;

estadojogo estado_atual = TELA_INICIAL;

Texture2D imgTELA_INICIAL = {0};
Texture2D imgFase[6] = {0};
int faseAtual = 0;

typedef struct 
{
    int inimigos;
    float velocidade;
    int vida;
    float tempoLimite;
} parametroFase;

parametroFase parametroFases[6] = {
    {0,    0.0f, 0, 0.0f},  
    {5,    1.5f, 1, 120.0f},
    {8,    2.0f, 1, 150.0f},
    {10,   2.2f, 2, 180.0f},
    {12,   2.5f, 2, 200.0f},
    {15,   3.0f, 3, 240.0f} 
};


void Estado_Iniciar(){
    imgTELA_INICIAL = LoadTexture("assets/tela_de_inicio.jpg");
    imgFase[1] = LoadTexture("assets/Fase1.jpg");
    imgFase[2] = LoadTexture("assets/Fase2.jpg");
    imgFase[3] = LoadTexture("assets/Fase3.jpg");
    imgFase[4] = LoadTexture("assets/Fase3.jpg");
    imgFase[5] = LoadTexture("assets/Fase3.jpg");
}

static void Estado_ComecoFase(int fase){
    LiberarInimigos();

    faseAtual = fase;
    estado_atual = (estadojogo)(FASE_1 + (fase - 1));

    tempoTotalPartida = 0.0f;
    partidaEmAndamento = true;

    j.x = 100;
    j.y = 100;
    j.hp = 100;
    j.hitbox.x = (float)j.x;
    j.hitbox.y = (float)j.y;

    parametroFase p = parametroFases[fase];
    for (int i = 0; i < p.inimigos; i++)
    {
        float px = (float)GetRandomValue(100,1180);
        float py = (float)GetRandomValue(100,629);
        AdicionarInimigo((Vector2){px, py}, p.velocidade, p.vida);

    }
}

void Estado_Desenhar(){
    char buf[64];

    switch (estado_atual)
    {
        case TELA_INICIAL:
        if (imgTELA_INICIAL.id != 0)
        {
            DrawTexture(imgTELA_INICIAL, 0, 0, WHITE);
        } else {
            ClearBackground(RAYWHITE);
        }
            DrawText("BONDED MVP - Pressione ESPAÇO para iniciar!!", 200, 200, 40, BLACK);
            DrawText("WASD para mover, clique esquerdo para atacar", 200, 260, 20, DARKGRAY);
            break;
        
        case FASE_1:
        case FASE_2:
        case FASE_3:
        case FASE_4:
        case FASE_FINAL:

        if (faseAtual >= 1 && faseAtual <= 5 && imgFase[faseAtual].id != 0){
            DrawTexture(imgFase[faseAtual], 0, 0, RAYWHITE);
        } else {
            ClearBackground(RAYWHITE);
        }

        desenhar_jogador(&j);
        DesenharInimigos();

        sprintf(buf, "Fase:%d", faseAtual);
        DrawText(buf, 10, 10, 22, BLACK);

        sprintf(buf, "HP:%d", j.hp);
        DrawText(buf, 10, 10, 24, RED);
        
        sprintf(buf, "TEMPO:%.2f", tempoTotalPartida);
        DrawText(buf, 10, 40, 20, BLACK);

        sprintf(buf, "INIMIGOS VIVOS:%d", total_inimigos_vivos);
        DrawText(buf, 10, 70, 20, BLACK);

        break;

        case TELA_GANHOU:
        ClearBackground(RAYWHITE);
        DrawText("VOCÊ VENCEU, PRESSIONE ESPAÇO PARA AVANÇAR", 200, 200, 40, GREEN);
        DesenharLeaderboard(200, 280);
        break;

        case TELA_PERDEU:
        ClearBackground(RAYWHITE);
        DrawText("VOCÊ PERDEU, PRESSIONE ESPAÇO PARA TENTAR NOVAMENTE", 200, 200, 40, RED);
        DesenharLeaderboard(200, 280);
        break;

        default:
        ClearBackground(RAYWHITE);
        DrawText("Estado Desconhecido", 200, 200, 20, BLACK);
        break;
    }
}

void Estado_Update(){

    float delta = GetFrameTime(); 

    if(partidaEmAndamento) tempoTotalPartida += delta;
    
    if (estado_atual == FASE_1 || estado_atual == FASE_2 || estado_atual == FASE_3 || estado_atual == FASE_4 || estado_atual == FASE_FINAL) {
        
        mover_jogador(&j);
        atualizar_jogador(&j);
    
        Vector2 posJogador = { (float)j.x, (float)j.y }; 
        AtualizarInimigos(posJogador, delta);
        VerificarColisao(j.hitbox);

        if (j.atacando) {

            Rectangle hitboxAtaque = {
                (float)j.x + (j.ultimadirecaoH == 1 ? j.framelargura : -j.larguraframeataque),
                (float)j.y,
                (float)j.larguraframeataque,
                (float)j.alturaframeataque
            };
            
            VerificarColisaoAtaque(hitboxAtaque, 1);
        }
    }

    if (j.hp <= 0){
        estado_atual = TELA_PERDEU;
        partidaEmAndamento = false;
        LiberarInimigos();
        return;
    }

    if(faseAtual >= 1 && faseAtual <= 5){
        float limite = parametroFases[faseAtual].tempoLimite;
        if(limite > 0 && tempoTotalPartida >= limite){
            estado_atual = TELA_PERDEU  ;
            partidaEmAndamento = false;
            LiberarInimigos();
            return;
        }

    if (total_inimigos_vivos <= 0 && (estado_atual == FASE_1 || estado_atual == FASE_2 ||
        estado_atual == FASE_3 || estado_atual == FASE_4 || estado_atual == FASE_FINAL)) {

        partidaEmAndamento = false;

        if (faseAtual < 5) {

            int proxima = faseAtual + 1;
            return;

        } else {
            
            estado_atual = TELA_GANHOU;
            ultimoTempo = tempoTotalPartida;
            SalvarLeaderboard(ultimoTempo);
            LiberarInimigos();
            return;
        }
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

void Estado_Finalizar(){
    if (imgTELA_INICIAL.id != 0) UnloadTexture(imgTELA_INICIAL);

    for (int i = 1; i <= 5; i++) {
        if (imgFase[i].id != 0) UnloadTexture(imgFase[i]);
    }

    LiberarInimigos();
}