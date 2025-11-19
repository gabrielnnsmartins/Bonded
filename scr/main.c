#include <raylib.h>
#include <stdbool.h>

#define SPRITE_SIZE 120
#define ANIMATION_FRAMES 3
#define FPS 5
#define FRAME_TIME (1.0f / FPS)

typedef enum {
    IDLE,
    WALKING_RIGHT,
    WALKING_LEFT
} AnimationState;

typedef struct {
    Vector2 position;
    Texture2D spriteSheet;
    AnimationState state;
    int currentFrame;
    float frameTimer;
    float speed;
    bool facingRight;
} Character;

// Inicializa o personagem
void InitCharacter(Character *character, const char *spritePath, Vector2 startPos) {
    character->position = startPos;
    character->spriteSheet = LoadTexture(spritePath);
    character->state = IDLE;
    character->currentFrame = 0;
    character->frameTimer = 0.0f;
    character->speed = 200.0f;
    character->facingRight = true;
}

// Atualiza a animação
void UpdateAnimation(Character *character, float deltaTime) {
    if (character->state == IDLE) {
        character->currentFrame = 0;
        return;
    }
    
    character->frameTimer += deltaTime;
    
    if (character->frameTimer >= FRAME_TIME) {
        character->frameTimer -= FRAME_TIME;
        character->currentFrame++;
        
        if (character->currentFrame >= ANIMATION_FRAMES) {
            character->currentFrame = 0;
        }
    }
}

// Atualiza o personagem
void UpdateCharacter(Character *character, float deltaTime) {
    Vector2 movement = {0, 0};
    bool isMoving = false;
    
    // Controles de movimento
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        movement.x += 1;
        isMoving = true;
        character->facingRight = true;
        character->state = WALKING_RIGHT;
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        movement.x -= 1;
        isMoving = true;
        character->facingRight = false;
        character->state = WALKING_LEFT;
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        movement.y += 1;
        isMoving = true;
    }
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        movement.y -= 1;
        isMoving = true;
    }
    
    // Normalizar movimento diagonal
    if (movement.x != 0 && movement.y != 0) {
        float length = sqrtf(movement.x * movement.x + movement.y * movement.y);
        movement.x /= length;
        movement.y /= length;
    }
    
    // Aplicar movimento
    if (isMoving) {
        character->position.x += movement.x * character->speed * deltaTime;
        character->position.y += movement.y * character->speed * deltaTime;
        
        // Manter na tela
        if (character->position.x < 0) character->position.x = 0;
        if (character->position.y < 0) character->position.y = 0;
        if (character->position.x > GetScreenWidth() - SPRITE_SIZE) 
            character->position.x = GetScreenWidth() - SPRITE_SIZE;
        if (character->position.y > GetScreenHeight() - SPRITE_SIZE) 
            character->position.y = GetScreenHeight() - SPRITE_SIZE;
    } else {
        character->state = IDLE;
    }
    
    UpdateAnimation(character, deltaTime);
}

// Desenha o personagem
void DrawCharacter(Character *character) {
    // Define o retângulo de origem na spritesheet
    // Assumindo que os frames estão organizados horizontalmente
    Rectangle sourceRec = {
        character->currentFrame * SPRITE_SIZE,  // X na spritesheet
        0,                                       // Y na spritesheet
        SPRITE_SIZE,                            // Largura
        SPRITE_SIZE                             // Altura
    };
    
    // Se estiver virado para a esquerda, inverte horizontalmente
    if (!character->facingRight) {
        sourceRec.width = -SPRITE_SIZE;
    }
    
    // Retângulo de destino na tela
    Rectangle destRec = {
        character->position.x,
        character->position.y,
        SPRITE_SIZE,
        SPRITE_SIZE
    };
    
    DrawTexturePro(
        character->spriteSheet,
        sourceRec,
        destRec,
        (Vector2){0, 0},
        0.0f,
        WHITE
    );
}

// Libera recursos
void UnloadCharacter(Character *character) {
    UnloadTexture(character->spriteSheet);
}

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