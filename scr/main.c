#include "raylib.h"
#include "jogador.h"
#include "estadojogo.h"
#include "inimigo.h"
#include "leaderboard.h"

Jogador j; 

int main(void) {

    const int larguraTela = 1280;
    const int alturaTela = 720;
    
    InitWindow(larguraTela, alturaTela, "Bonded - MVP");
    TraceLog(LOG_INFO, "Carregando texturas...");
    Texture2D test = LoadTexture("assets/tela_de_inicio.jpg");
    if (test.id == 0) {
        TraceLog(LOG_ERROR, "FALHOU ao carregar: assets/teste.png");
    } else {
        TraceLog(LOG_INFO, "Carregou normalmente!");
    }
    UnloadTexture(test);

    SetTargetFPS(60);

    Estado_Iniciar();
    iniciar_jogador(&j, "assets/personagem.png");
    CarregarInimigoTex("assets/Inimigo.png"); 
    CarregarLeaderboard();

    while (!WindowShouldClose()) {

        Estado_Update(); 
        
        BeginDrawing();
        ClearBackground(RAYWHITE);

        Estado_Desenhar();

        EndDrawing();
    }

    unload_jogador(&j);
    UnloadTexInimigo();
    Estado_Finalizar();
    CloseWindow();
    return 0;
}
