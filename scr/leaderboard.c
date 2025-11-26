#include "leaderboard.h" 
#include <string.h>     

bool partidaEmAndamento = false;
float ultimoTempo = 0.0f;
float temposLeaderboard[5];
int totalTempos = 0;

void CarregarLeaderboard() {
    totalTempos = 0;
    FILE *f = fopen("leaderboard.txt", "r");
    if (f != NULL) {
        while (totalTempos < 5 && fscanf(f, "%f", &temposLeaderboard[totalTempos]) == 1) {
            totalTempos++;
        }
        fclose(f);
    }
}

void SalvarLeaderboard(float tempoFinal) {
    float tempos[6];
    int count = 0;

    FILE *f = fopen("leaderboard.txt", "r");
    if (f != NULL) {
        while (count < 5 && fscanf(f, "%f", &tempos[count]) == 1) count++;
        fclose(f);
    }

    tempos[count++] = tempoFinal;

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (tempos[i] > tempos[j]) {
                float aux = tempos[i];
                tempos[i] = tempos[j];
                tempos[j] = aux;
            }
        }
    }

    if (count > 5) count = 5;

    f = fopen("leaderboard.txt", "w");
    for (int i = 0; i < count; i++) fprintf(f, "%.2f\n", tempos[i]);
    fclose(f);
}

void DesenharLeaderboard(int x, int y) {
    CarregarLeaderboard();
    DrawText("TOP 5 MELHORES TEMPOS:", x, y, 30, YELLOW);
    
    for (int i = 0; i < totalTempos; i++) {
        char linha[64];
        sprintf(linha, "%dº - %.2f segundos", i + 1, temposLeaderboard[i]);
        DrawText(linha, x, y + 50 + i * 40, 25, WHITE);
    }
    
    if (ultimoTempo > 0) {
        char msgFinal[64];
        sprintf(msgFinal, "SEU ULTIMO TEMPO: %.2f segundos", ultimoTempo);
        DrawText(msgFinal, x, y + 350, 28, GREEN);
    }
}