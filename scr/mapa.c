#include "mapa.h"
#include "raylib.h"

int MAPA[Mapa_Altura][Mapa_Largura] = {0}; 

void CarregarMapa(){
    
    for (int y = 0; y < Mapa_Altura; y++)
    {
        for (int x = 0; x < Mapa_Largura; x++)
        {
            MAPA[y][x] = 0;
        }
    }

    for (int x = 0; x < Mapa_Largura; x++)
    {
        MAPA[0][x] = 1;
        MAPA[Mapa_Altura-1][x] = 1;
    }

    for (int y = 0; y < Mapa_Altura; y++)
    {
        MAPA[y][0] = 1;
        MAPA[y][Mapa_Largura - 1] = 1; 
    }
}

void DesenharMapa(){

    for (int y = 0; y < Mapa_Altura; y++)
    {
        for (int x = 0; x < Mapa_Largura; x++)
        {
            int tile = MAPA[y][x];
            
            if (tile == 1) { 
                DrawRectangle(
                    x * TAMANHO_TILE,
                    y * TAMANHO_TILE,
                    TAMANHO_TILE,
                    TAMANHO_TILE,
                    BROWN 
                );
            }
        }
    }
}

bool VerificarColisaoMapa(int x_pixel, int y_pixel) {
    
    int tile_x = x_pixel / TAMANHO_TILE;
    int tile_y = y_pixel / TAMANHO_TILE;

    if (tile_x < 0 || tile_x >= Mapa_Largura || 
        tile_y < 0 || tile_y >= Mapa_Altura) {
        return true; 
    }

    if (MAPA[tile_y][tile_x] == 1) { 
        return true; 
    }
    
    return false; 
}