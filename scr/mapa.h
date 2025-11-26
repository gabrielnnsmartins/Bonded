/*#ifndef Mapa_h
#define Mapa_h

#include<raylib.h>
#include<stdbool.h>

#define Mapa_Largura 20
#define Mapa_Altura 15
#define Tile_Tamanho 48

extern int MAPA[Mapa_Altura][Mapa_Largura];

void CarregarMapa();
void DesenharMapa();
bool Mapa_TileSolido(int tx, int ty);
bool Mapa_ChecarColisaoJogador(int nx, int ny, int largura, int altura);

#endif*/