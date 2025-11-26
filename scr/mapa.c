#include "mapa.h"

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
                DrawRectangle(x * Tile_Tamanho, y * Tile_Tamanho, Tile_Tamanho, Tile_Tamanho, DARKGRAY);
            } else {
                DrawRectangle(x * Tile_Tamanho, y * Tile_Tamanho, Tile_Tamanho, Tile_Tamanho, LIGHTGRAY);
            }
        }
        
    }
    
}