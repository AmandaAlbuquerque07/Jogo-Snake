#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "listacobra.h"

void FSVazia(ListaSnake *Snake){
    Snake->Cabeca = (SnakeApontador)malloc(sizeof(CelulaSnake));
    Snake->Cauda = Snake->Cabeca;
    Snake->Cabeca->Prox = NULL;
    Snake->Comprimento = 0;
}

void IniciaSnake(Jogo *j){
    FSVazia(&j->snake); //Cria uma lista vazia.
    
    //Cria a cauda:
    j->snake.Cabeca->Prox = (SnakeApontador)malloc(sizeof(CelulaSnake));
    j->snake.Cauda = j->snake.Cabeca->Prox;
    j->snake.Cauda->Prox = NULL;

    //Cabeça:
    j->snake.Cabeca->body.pos = (Rectangle) {LARGURA/2 - STD_SIZE_X, ALTURA - STD_SIZE_Y -10, STD_SIZE_X, STD_SIZE_Y};
    j->snake.Cabeca->body.direcao = 0;
    j->snake.Cabeca->body.color = SNAKE_COLOR;

    //Cauda (logo após a cabeça)
    j->snake.Cauda->body.pos = (Rectangle){LARGURA/2 - STD_SIZE_X, ALTURA/2, STD_SIZE_X, STD_SIZE_Y};
    j->snake.Cauda->body.direcao = 0;
    j->snake.Cauda->body.color = SNAKE_COLOR;

    j->snake.dirX = 0;
    j->snake.dirY = -1; // começa subindo
    j->snake.Comprimento = 2;
}

void AumentaSnake(Jogo *j){
    SnakeApontador novo = (SnakeApontador)malloc(sizeof(CelulaSnake));

    novo->body.pos = j->snake.Cabeca->Prox->body.pos; 
    novo->body.direcao = j->snake.Cabeca->Prox->body.direcao;
    novo->body.color = SNAKE_COLOR;

    //Insere na Lista na primeira posição logo após a cabeça:
    novo->Prox = j->snake.Cabeca->Prox;
    j->snake.Cabeca->Prox = novo;
    j->snake.Comprimento++;

}

void IniciaBordas(Jogo *j){
    //Borda de cima
    j->bordas[0].pos = (Rectangle) {0, 0, LARGURA, 10};
    //Borda da direita
    j->bordas[1].pos = (Rectangle) {LARGURA - 10, 0, 10, ALTURA};
    //Borda de baixo
    j->bordas[2].pos = (Rectangle) {0, ALTURA - 10, LARGURA, 10};
    //Borda da esquerda
    j->bordas[3].pos = (Rectangle) {0, 0, 10, ALTURA};
}

void IniciaFood(Jogo *j){
    j->food.pos = (Rectangle) {(float)(rand() % ((ALTURA - 20) / STD_SIZE_Y) * STD_SIZE_Y + 10), (float)(rand() % ((ALTURA - 20) / STD_SIZE_Y) * STD_SIZE_Y + 10), STD_SIZE_X, STD_SIZE_Y};
    j->food.color = FOOD_COLOR;
}

void IniciaJogo(Jogo *j){
    IniciaBordas(j);
    IniciaSnake(j);
    IniciaFood(j);
    j->tempo = GetTime();
}

void DesenhaSnake(Jogo *j) {
    SnakeApontador aux = j->snake.Cabeca;
    while(aux != NULL) {
        DrawRectangleRec(aux->body.pos, aux->body.color);
        aux = aux->Prox;
    }
}

void DesenhaFood(Jogo *j){
    DrawRectangleRec(j->food.pos, j->food.color);
}

void DesenhaBordas(Jogo *j){
    //Desenha as barreiras nas bordas
    for (int i = 0; i < 4; i++){
        DrawRectangleRec(j->bordas[i].pos, LIGHTGRAY);
    }
}

void DesenhaJogo(Jogo *j){
    DesenhaBordas(j);
    DesenhaSnake(j);
    DesenhaFood(j);
}

void AtualizaDirecao(Jogo *j){
    if(GetTime() - j->cooldown < COOLDOWN) return;

    //Atualiza para qual direção a cobra vai  
   if(IsKeyDown(KEY_UP) && j->snake.dirY != 1){
        j->snake.dirX = 0;
        j->snake.dirY = -1;
        j->cooldown =  GetTime();
    }
    // Para direita
    if(IsKeyDown(KEY_RIGHT) && j->snake.dirX != -1){
        j->snake.dirX = 1;
        j->snake.dirY = 0;
        j->cooldown =  GetTime();
    }
    // Para baixo
    if(IsKeyDown(KEY_DOWN) && j->snake.dirY != -1){
        j->snake.dirX = 0;
        j->snake.dirY = 1;
        j->cooldown =  GetTime();
    }
    // Para esquerda
    if(IsKeyDown(KEY_LEFT) && j->snake.dirX != 1){
        j->snake.dirX = -1;
        j->snake.dirY = 0;
        j->cooldown =  GetTime();
    }
}

void AtualizaPosSnake(Jogo *j) {
    ListaSnake *snake = &j->snake; // pega referência da cobra
    SnakeApontador atual = snake->Cabeca;

    // Guarda posição da cabeça antes de mover
    float prevX = atual->body.pos.x;
    float prevY = atual->body.pos.y;

    // Move cabeça
    atual->body.pos.x += snake->dirX * STD_SIZE_X;
    atual->body.pos.y += snake->dirY * STD_SIZE_Y;

    atual = atual->Prox;

    // Move o resto do corpo seguindo a cabeça
    while(atual != NULL) {
        float tempX = atual->body.pos.x;
        float tempY = atual->body.pos.y;

        atual->body.pos.x = prevX;
        atual->body.pos.y = prevY;

        prevX = tempX;
        prevY = tempY;

        atual = atual->Prox;
    }
}

void AtualizaRodada(Jogo *j){
    AtualizaDirecao(j);
    if (GetTime() - j->tempo > TEMPO){
        AtualizaPosSnake(j);
        j->tempo = GetTime();
        j->cooldown = COOLDOWN;
    }
}

int ColisaoFood(Jogo *j){
    if (CheckCollisionRecs(j->snake.Cabeca->body.pos, j->food.pos)){
        return 1;
    }
    return 0;
}

int ColisaoBordas(Jogo *j){
    if (CheckCollisionRecs(j->snake.Cabeca->body.pos, j->bordas[0].pos)){
        return 1;
    }else if(CheckCollisionRecs(j->snake.Cabeca->body.pos, j->bordas[1].pos)){
        return 1;
    }else if(CheckCollisionRecs(j->snake.Cabeca->body.pos, j->bordas[2].pos)){
        return 1;
    }else if(CheckCollisionRecs(j->snake.Cabeca->body.pos, j->bordas[3].pos)){
        return 1;
    }else{
        return 0;
    }
}
