#include <raylib.h>
#include "jogador.h"
#include <stdio.h>

const float duracaoataque;

//Inicia o jogador na tela, com sua posição, textura e hp
void iniciar_jogador(Jogador *j, const char *spritePath) {

    j->x= 100;
    j->y= 100;
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
    j->spriteataque = LoadTexture("assets/ataque.png");

    j->larguraframeataque = j->spriteataque.width;
    j->alturaframeataque = j->spriteataque.height;


}

// atualiza os frames fazendo a animação
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

            if (j->frameatual >= j->numframes /2){
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

//atualiza a posição do jogador, fazendo ele mover
void mover_jogador(Jogador *j){

    int velocidade = 4;
    int nx = j->x;
    int ny = j->y;

    if(j->atacando)return;

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

    j->x = nx;
    j->y = ny;

    j->hitbox.x = (float)j->x;
    j->hitbox.y = (float)j->y;

}

//Põe o jogador com textura na tela
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

// some com o jogador
void unload_jogador(Jogador *j){
    UnloadTexture(j->sprite);
}
