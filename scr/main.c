#include <raylib.h>

int main(){
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Bonded");
    SetTargetFPS(60);
    
     while (!WindowShouldClose())
    {    
        BeginDrawing();
            ClearBackground(Bonded/cenario.png);
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}