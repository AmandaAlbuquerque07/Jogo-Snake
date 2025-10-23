#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "listacobra.h"

int main(){
    Jogo jogo;
    int gameOver = 10;

    //Cria a janela;
    InitWindow(LARGURA, ALTURA, "Snake Game");
    Texture2D imagem = LoadTexture("maça.png");
    SetTargetFPS(50);
    srand(time(NULL));
    
    IniciaJogo(&jogo);
    while (!WindowShouldClose()){ //fecha se a tecla esc for precionada
        BeginDrawing();
        ClearBackground(BLACK);
        
        if (gameOver){
            DesenhaJogo(&jogo);
            AtualizaRodada(&jogo);
            if (ColisaoFood(&jogo)){
                IniciaFood(&jogo);
            }
            if (ColisaoBordas(&jogo)){
                gameOver=0;
            }
        } else {
            DrawText("Agora é a sua vez!", 150, 200, 40, WHITE);
            DrawText("Continue", 265, 400, 30, WHITE);
            if (IsKeyPressed(KEY_ENTER)){
                IniciaJogo(&jogo);
                gameOver = 1;
            }
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

