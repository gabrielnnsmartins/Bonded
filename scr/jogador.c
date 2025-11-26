#include <raylib.h>
#include "jogador.h"
#include <stdio.h>

// Inicia o jogador na tela
void iniciar_jogador(Jogador *j, const char *spritePath) {
    j->x = 100;
    j->y = 100;
    j->hp = 100;

    j->spritePath = spritePath;
    j->sprite = LoadTexture(spritePath);

    if (j->sprite.id == 0){
        printf("Erro ao carregar textura do jogador: %s\n", spritePath);
    }

    j->numframes = 6;
    j->framelargura = j->sprite.width / j->numframes;
    j->frameatual = 0;
    j->tempoanimacao = 0.0f;
    j->framedelay = 0.1f;
    j->movendo = false;
    j->ultimadirecaoH = 1;  

    j->hitbox = (Rectangle) {
        (float)j->x, 
        (float)j->y, 
        (float)j->framelargura, 
        (float)j->sprite.height 
    };

    j->atacando = false;
    j->tempoataque = 0.0f;
    j->duracaoataque = 0.25f;

    j->spriteataque = LoadTexture("assets/ataque.png");
    if(j->spriteataque.id == 0){
        printf("Erro ao carregar sprite de ataque: assets/ataque.png\n");
    }

    j->larguraframeataque = j->spriteataque.width;
    j->alturaframeataque = j->spriteataque.height;
}

// Atualiza animações e timers (Removi a lógica de colisão errada daqui)
void atualizar_jogador(Jogador *j){
    if (j->atacando){
        j->tempoataque += GetFrameTime();
        if (j->tempoataque >= j->duracaoataque)
        {
            j->atacando = false;
            j->tempoataque = 0.0f;
        }
        return; 
    }

    if (j->movendo){
        float tempoframe = GetFrameTime();
        j->tempoanimacao += tempoframe;

        if (j->tempoanimacao >= j->framedelay){
            j->tempoanimacao = 0.0f;
            j->frameatual++;

            if (j->frameatual >= j->numframes / 2){
                j->frameatual = 0;
            }
        }
    } else {
        j->frameatual = 0;
        j->tempoanimacao= 0.0f;
    }
}

void atacar (Jogador *j){
    if (!j->atacando){
        j->atacando = true;
        j->tempoataque = 0.0f;
    }
}

// Atualiza a posição e APHERE (AQUI) aplicamos os limites da tela
void mover_jogador(Jogador *j){
    int velocidade = 4;
    int nx = j->x;
    int ny = j->y;

    if(j->atacando) return;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        atacar(j);
        return;
    }
    
    j->movendo = false;

    if (IsKeyDown(KEY_W)) ny -= velocidade;
    if (IsKeyDown(KEY_S)) ny += velocidade;

    if (IsKeyDown(KEY_A)){
        nx -= velocidade;
        j->ultimadirecaoH = -1;
    } 
    if (IsKeyDown(KEY_D)){
        nx += velocidade;
        j->ultimadirecaoH = 1;
    } 

    if ((nx != j->x) || (ny != j->y)){
        j->movendo = true;
    }

    // Aplica a nova posição temporariamente
    j->x = nx;
    j->y = ny;

    // --- LÓGICA DE LIMITES DA TELA ---
    // Ajustada para usar as variáveis da struct 'j' (x, y, framelargura)
    
    // Teto (Horizonte em 105px)
    if (j->y < 105) j->y = 105;

    // Chão (720px - altura da imagem)
    if (j->y > 720 - j->sprite.height) j->y = 720 - j->sprite.height;

    // Parede Esquerda (0)
    if (j->x < 0) j->x = 0;

    // Parede Direita (1280px - largura do frame atual)
    if (j->x > 1280 - j->framelargura) j->x = 1280 - j->framelargura;
    // ---------------------------------

    // Atualiza a hitbox com a posição final corrigida
    j->hitbox.x = (float)j->x;
    j->hitbox.y = (float)j->y;
}

void desenhar_jogador(const Jogador *j){
    int offset_frame_parado = ((j->ultimadirecaoH == 1) ? 3 : 0);
    int frameIndex;

    if(j->atacando){
        float offsetX = (j->ultimadirecaoH == 1) ? j->framelargura : -j->larguraframeataque;
        Rectangle destRec = {
            (float)j->x + offsetX,
            (float)j->y,
            (float)j->larguraframeataque,
            (float)j->alturaframeataque
        };

        DrawTextureRec(
            j->spriteataque,
            (Rectangle){0, 0, (float)j->larguraframeataque, (float)j->alturaframeataque},
            (Vector2){ destRec.x, destRec.y},
            WHITE
        );
        return;
    }

    if (j->movendo){
        frameIndex = j->frameatual + ((j->ultimadirecaoH == 1) ? 3 : 0);
    } else {
        frameIndex = offset_frame_parado;
    }

    Rectangle frameSource ={
        (float)(frameIndex * j->framelargura),
        0.0f,
        (float)j->framelargura,
        (float)j->sprite.height
    };

    Rectangle frameDestino ={
        (float)j->x,
        (float)j->y,
        (float)j->framelargura,
        (float)j->sprite.height
    };

    DrawTexturePro(
        j->sprite,
        frameSource,
        frameDestino,
        (Vector2){0 , 0},
        0.0f,
        WHITE
    );
}

void unload_jogador(Jogador *j){
    UnloadTexture(j->sprite);
    if (j->spriteataque.id != 0) UnloadTexture(j->spriteataque);
}