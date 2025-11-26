#include "raylib.h"
#include "jogador.h"
#include "leaderboard.h"
#include "estadojogo.h"

Jogador j;

int main(void) {

    // Inicialização da Janela, jogador e fps
    InitWindow(1280, 720, "Bonded - MVP");
    iniciar_jogador(&j, "assets/personagem.png");
    Estado_Iniciar();
    SetTargetFPS(60);

    // Loop Principal do Jogo
    while (!WindowShouldClose()) {

        Estado_Update();
        mover_jogador(&j);
        atualizar_jogador(&j);
        
        BeginDrawing();
        ClearBackground(RAYWHITE);

        desenhar_jogador(&j);

        EndDrawing();


    }

    unload_jogador(&j);
    CloseWindow();
    return 0;
}

