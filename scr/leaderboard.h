#ifndef leaderboard_h
#define leaderboard_h

#include <raylib.h>
#include <stdio.h>

extern float tempoTotalPartida;
extern bool partidaEmAndamento;
extern float ultimoTempo;
extern float temposLeaderboard[5];
extern int totalTempos;

void CarregarLeaderboard();
void SalvarLeaderboard(float tempoFinal);
void DesenharLeaderboard(int x, int y);

#endif