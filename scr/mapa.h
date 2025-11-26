#ifndef MAPA_H
#define MAPA_H

#include <stdbool.h>

#define Mapa_Largura 40 
#define Mapa_Altura 23 


#define TAMANHO_TILE 32 

extern int MAPA[Mapa_Altura][Mapa_Largura];

void CarregarMapa();
void DesenharMapa();
bool VerificarColisaoMapa(int x_pixel, int y_pixel);

#endif 