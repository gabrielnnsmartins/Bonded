#include "estadojogo.h"
#include "jogador.h"
#include "leaderboard.h"

estadojogo estado_atuaç = TELA_INICIAL;

extern Jogador j;

void Estado_Iniciar(){
    switch (estado_atual)
    {
    case TELA_INICIAL:
        
        if (IsKeyPressed(KEY_SPACE))
        {
            estado_atual = FASE_1;
        }
        
        break;
    
    default:
        break;
    }
}