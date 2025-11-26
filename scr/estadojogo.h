#ifndef estadojogo_h
#define estadojogo_h
#include <raylib.h>

typedef enum{
    TELA_INICIAL,
    FASE_1,
    FASE_2,
    FASE_3,
    FASE_4,
    FASE_FINAL,
    TELA_FINAL
} estadojogo;

extern estadojogo estado_atual;

void Estado_Iniciar();
void Estado_Update();
void Estado_Desenhar();

#endif