#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "listacobra.h"
#define TAMANHO_CELULA 40


void FSVazia(ListaSnake *Snake){
    Snake->Cabeca = (SnakeApontador)malloc(sizeof(CelulaSnake));
    Snake->Cauda = Snake->Cabeca;
    Snake->Cabeca->Prox = NULL;
    Snake->Comprimento = 0;
}

void IniciaSnake(Jogo *j){
    FSVazia(&j->snake); //Cria uma lista vazia.

    Texture2D cabeca = LoadTexture("Assets/cabeca.png");
    Texture2D rabo = LoadTexture("Assets/rabo.png");
    
    //Cria a cauda:
    j->snake.Cabeca->Prox = (SnakeApontador)malloc(sizeof(CelulaSnake));
    j->snake.Cauda = j->snake.Cabeca->Prox;
    j->snake.Cauda->Prox = NULL;

    //Cabeça:
    j->snake.Cabeca->body.pos = (Rectangle) {LARGURA/2 - STD_SIZE_X, ALTURA - STD_SIZE_Y -10, STD_SIZE_X, STD_SIZE_Y};
    //posição inicial
    j->snake.Cabeca->body.direcao = 0;
    j->snake.Cabeca->body.color = cabeca;


    //Cauda (logo atrás da cabeça)
    j->snake.Cauda->body.pos = (Rectangle){ j->snake.Cabeca->body.pos.x, j->snake.Cabeca->body.pos.y + STD_SIZE_Y, STD_SIZE_X, STD_SIZE_Y};

    j->snake.Cauda->body.direcao = 0;
    j->snake.Cauda->body.color = rabo;

    j->snake.dirX = 0;
    j->snake.dirY = -1; // começa subindo
    j->snake.Comprimento = 2;
}

void AumentaSnake(Jogo *j){
    Texture2D corpo = LoadTexture("Assets/corpo.png");
    SnakeApontador novo = (SnakeApontador)malloc(sizeof(CelulaSnake));

    novo->body.pos = j->snake.Cabeca->Prox->body.pos; 
    novo->body.direcao = j->snake.Cabeca->Prox->body.direcao;
    novo->body.color = corpo;

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

void IniciaBarreiras1(Jogo *j, Texture2D pedras, Texture2D pedras1, Texture2D pedras2){
    j->barreiras[0].pos = (Rectangle) {LARGURA- 530, ALTURA-490, 40, 240};
    j->barreiras[1].pos = (Rectangle) {LARGURA - 130, ALTURA-490, 40, 240};
}


void IniciaFood(Jogo *j){
    int colisao;
    do {
        colisao = 0; // assume que não há colisão
        j->food.pos.x = (float)((rand() % ((LARGURA - 20) / STD_SIZE_X)) * STD_SIZE_X + 10);
        j->food.pos.y = (float)((rand() % ((ALTURA - 20) / STD_SIZE_Y)) * STD_SIZE_Y + 10);
        //STD size é o tamanho do quadradinho
        j->food.pos.width = STD_SIZE_X;
        j->food.pos.height = STD_SIZE_Y;

        // Verifica se a nova posição colide com a cobra
        SnakeApontador aux = j->snake.Cabeca;
        while(aux != NULL){
            if(CheckCollisionRecs(j->food.pos, aux->body.pos)){
                colisao = 1; // houve colisão, precisa gerar outra posição
                break;
            }
            aux = aux->Prox;
        } 
        if(colisao == 0){
            for(int i=0; i<10; i++){
                if(CheckCollisionRecs(j->food.pos, j->barreiras[i].pos)){
                    colisao = 1; // houve colisão, precisa gerar outra posição
                    break;
                }
            }
        }    
    } while(colisao); // repete até não colidir

    j->food.color = FOOD_COLOR;
}


void IniciaJogo(Jogo *j){
    IniciaBordas(j);
    IniciaSnake(j);
    IniciaFood(j);
    j->tempo = GetTime();
}


void DesenhaSnake(Jogo *j) {

    SnakeApontador k = j->snake.Cabeca;
    Texture2D cabeca = LoadTexture("Assets/cabeca.png");
    DrawTexturePro(
        cabeca,
        (Rectangle){0, 0, k->body.color.width, k->body.color.height}, //imagem
        (Rectangle){k->body.pos.x, k->body.pos.y, STD_SIZE_X, STD_SIZE_Y}, //pra onde ele vai na tela!
        (Vector2){0, 0}, //origem
        0.0f, //rotação?
        WHITE
        );

    SnakeApontador aux = j->snake.Cabeca->Prox;
    Texture2D corpo = LoadTexture("Assets/corpo.png");
    aux->body.color=corpo;

    while(aux->Prox != NULL) {
        DrawTexturePro(
        aux->body.color,
        (Rectangle){0, 0, aux->body.color.width, aux->body.color.height}, //imagem
        (Rectangle){aux->body.pos.x, aux->body.pos.y, STD_SIZE_X, STD_SIZE_Y}, //pra onde ele vai na tela!
        (Vector2){0, 0}, //origem
        0.0f, //rotação?
        WHITE
        );
        //função da raylib

        aux = aux->Prox;
    }

    SnakeApontador i = j->snake.Cauda;
    Texture2D rabo = LoadTexture("Assets/rabo.png");
    DrawTexturePro(
        rabo,
        (Rectangle){0, 0, i->body.color.width, i->body.color.height}, //imagem
        (Rectangle){i->body.pos.x, i->body.pos.y, STD_SIZE_X, STD_SIZE_Y}, //pra onde ele vai na tela!
        (Vector2){0, 0}, //origem
        0.0f, //rotação?
        WHITE
        );
}

void DesenhaFood(Jogo *j, Texture2D img){
    // Desenha a comida usando a textura da png que é redimensionada para STD_SIZE_X e STD_SIZE_Y
    DrawTexturePro(
        img,
        (Rectangle){0, 0, img.width, img.height},                   // Fonte (toda a textura)
        (Rectangle){j->food.pos.x, j->food.pos.y, STD_SIZE_X, STD_SIZE_Y}, // Destino na tela
        (Vector2){0, 0},                                            // Origem para rotação
        0,                                                           // Rotação
        WHITE                                                        // Cor
    );
}

void DesenhaBarreiras1(Jogo *j, Texture2D pedras, Texture2D pedras1, Texture2D pedras2){
    //Desenha as barreiras nas bordas
    for (int i = 0; i < 2; i++){
        DrawRectangleRec(j->barreiras[i].pos, WHITE);
    }
}



void DesenhaBarreiras3(Jogo *j) {

    Texture2D tubaraoD = LoadTexture("Assets/tubarao2.png");
    Texture2D tubaraoE = LoadTexture("Assets/tubarao1.png");

        DrawTexturePro(
            tubaraoD,
            (Rectangle){
                0,
                0,
                tubaraoD.width,
                tubaraoD.height
            }, // imagem inteira

            (Rectangle){
                j->barreiras[0].pos.x,
                j->barreiras[0].pos.y,
                160,      
                80
            }, // onde desenhar

            (Vector2){0, 0},   // origem
            0.0f,              // rotação
            WHITE
        );
        DrawTexturePro(
            tubaraoE,
            (Rectangle){
                0,
                0,
                tubaraoE.width,
                tubaraoE.height
            }, // imagem inteira

            (Rectangle){
                j->barreiras[1].pos.x,
                j->barreiras[1].pos.y,
                160,      
                80
            }, // onde desenhar

            (Vector2){0, 0},   // origem
            0.0f,              // rotação
            WHITE
        );
        /*Parte da Amanda:
        void DesenhaBarreiras3(Jogo *j){
        //Desenha as barreiras nas bordas:
            for (int i = 0; i < 2; i++){
            DrawRectangleRec(j->barreiras[i].pos, WHITE);
            }
        }   
        */
    
}



void DesenhaJogo(Jogo *j, Texture2D maca){
    //DesenhaBordas(j);
    DesenhaSnake(j);
    DesenhaFood(j, maca);
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

void AtualizaBarreiras3(Jogo *j){
    static int iniciado = 0;

    // Inicializa apenas uma vez
    if (!iniciado){
        // Barreira 0: direita → esquerda
        j->barreiras[0].pos = (Rectangle){LARGURA, ALTURA - 530, 160, 80};
        j->barreiras[0].pos = (Rectangle){LARGURA, ALTURA - 530, 160, 80};
        j->barreiras[0].velocidade = -3;

        // Barreira 1: esquerda → direita
        j->barreiras[1].pos = (Rectangle){-LARGURA, ALTURA - 250, 160, 80};
        j->barreiras[1].pos = (Rectangle){-LARGURA, ALTURA - 250, 160, 80};
        j->barreiras[1].velocidade = 3;

        iniciado = 1;
    }

    // Movimento das barreiras
    for (int i = 0; i < 2; i++) {
        j->barreiras[i].pos.x += j->barreiras[i].velocidade;

        // Se sair pela direita, volta da esquerda
        if (j->barreiras[i].pos.x > LARGURA) {
            j->barreiras[i].pos.x = -j->barreiras[i].pos.width;
        }

        // Se sair pela esquerda, volta da direita
        if (j->barreiras[i].pos.x < -j->barreiras[i].pos.width) {
            j->barreiras[i].pos.x = LARGURA;
        }
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

int ColisaoBarreiras1(Jogo *j){
    if (CheckCollisionRecs(j->snake.Cabeca->body.pos, j->barreiras[0].pos)){
        return 1;
    }else if(CheckCollisionRecs(j->snake.Cabeca->body.pos, j->barreiras[1].pos)){
        return 1;
    }else{
        return 0;
    }
}

int ColisaoBarreiras3(Jogo *j){
    SnakeApontador aux = j->snake.Cabeca;

    // Passa por TODOS os segmentos da cobra
    while (aux != NULL){
        for (int i = 0; i < 2; i++){    // suas 2 barreiras móveis
            if (CheckCollisionRecs(aux->body.pos, j->barreiras[i].pos)){
                return 1;   // bateu → morre
            }
        }
        aux = aux->Prox;
    }

    return 0; // Não colidiu
}


void ColisaoBordas(Jogo *j) {
    // Se sair pela esquerda, reaparece na direita
    if (j->snake.Cabeca->body.pos.x < 0) {
        j->snake.Cabeca->body.pos.x = LARGURA - TAMANHO_CELULA;
    }
    // Se sair pela direita, reaparece na esquerda
    else if (j->snake.Cabeca->body.pos.x >= LARGURA) {
        j->snake.Cabeca->body.pos.x = 0;
    }

    // Se sair por cima, reaparece embaixo
    if (j->snake.Cabeca->body.pos.y < 0) {
        j->snake.Cabeca->body.pos.y = ALTURA - TAMANHO_CELULA;
    }
    // Se sair por baixo, reaparece em cima
    else if (j->snake.Cabeca->body.pos.y >= ALTURA) {
        j->snake.Cabeca->body.pos.y = 0;
    }
}

int ColisaoSnake(Jogo *j){
    SnakeApontador aux = j->snake.Cabeca->Prox; // começa depois da cabeça
    float headX = j->snake.Cabeca->body.pos.x;
    float headY = j->snake.Cabeca->body.pos.y;

    while(aux != NULL){
        if(aux->body.pos.x == headX && aux->body.pos.y == headY){
            return 1; // Colidiu com o corpo
        }
        aux = aux->Prox;
    }
    return 0; // sem colisão
}

void FreeLista(ListaSnake *Snake){
    SnakeApontador atual = Snake->Cabeca;
    SnakeApontador aux;
    while(atual != NULL){
        aux = atual;
        atual = atual->Prox;
        UnloadTexture(aux->body.color);
        free(aux);
        
    }
    Snake->Comprimento = 0;
}
