#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "listacobra.h"
#define TAMANHO_CELULA 40

#include "giracobra.h"
#define CIMA     0
#define DIREITA  1
#define BAIXO    2
#define ESQUERDA 3

void FSVazia(ListaSnake *Snake){
    Snake->Cabeca = NULL;
    Snake->Cauda  = NULL;
    Snake->Comprimento = 0;
}

void CarregaTexturas(Jogo *j){
    j->tex.Cabeca = LoadTexture("Assets/cabecanovo4.png");
    j->tex.Corpo  = LoadTexture("Assets/corponovo4.png");
    j->tex.Rabo   = LoadTexture("Assets/rabonovo4.png");

    j->tex.pedras    = LoadTexture("Assets/pedras.png");
    j->tex.pedras1   = LoadTexture("Assets/pedras1.png");
    j->tex.pedras2   = LoadTexture("Assets/pedras2.png");

    j->tex.tubaraoD  = LoadTexture("Assets/tubarao2.png");
    j->tex.tubaraoE  = LoadTexture("Assets/tubarao1.png");

    j->tex.Food = LoadTexture("Assets/maca.png");
}


void IniciaSnake(Jogo *j){
    //teste
    printf("Comprimento: %d\n", j->snake.Comprimento);
    CelulaSnake *c = j->snake.Cabeca;
        while(c){
        printf("Segmento: x=%f y=%f dir=%d\n", c->body.pos.x, c->body.pos.y, c->body.direcao);
        c = c->Prox;
        }
    Texture2D cabeca = LoadTexture("Assets/cabeca.png");
    Texture2D rabo   = LoadTexture("Assets/rabo.png");

    j->snake.Cabeca = (SnakeApontador)malloc(sizeof(CelulaSnake));
    memset(j->snake.Cabeca, 0, sizeof(CelulaSnake));
    j->snake.Cauda  = (SnakeApontador)malloc(sizeof(CelulaSnake));
    memset(j->snake.Cauda, 0, sizeof(CelulaSnake));

    j->snake.Cabeca->Prox = j->snake.Cauda;
    j->snake.Cauda->Prox  = NULL;

    j->snake.Cabeca->body.pos = (Rectangle){ j->LARGURA/2 - STD_SIZE_X, j->ALTURA - STD_SIZE_Y - 10, STD_SIZE_X, STD_SIZE_Y };
    j->snake.Cabeca->body.direcao = CIMA;
    j->snake.Cabeca->body.color   = cabeca;

    j->snake.Cauda->body.pos = (Rectangle){ j->snake.Cabeca->body.pos.x, j->snake.Cabeca->body.pos.y + STD_SIZE_Y, STD_SIZE_X, STD_SIZE_Y };
    j->snake.Cauda->body.direcao = CIMA;
    j->snake.Cauda->body.color   = rabo;

    j->snake.Comprimento = 2;
}

void AumentaSnake(Jogo *j){
    Texture2D corpo = LoadTexture("Assets/corpo.png");
    SnakeApontador novo  = malloc(sizeof(CelulaSnake));
    memset(novo, 0, sizeof(CelulaSnake));

    // posição inicial: logo atrás do rabo atual
    novo->body.pos = j->snake.Cauda->body.pos;
    switch (j->snake.Cauda->body.direcao) {
        case CIMA:     novo->body.pos.y += STD_SIZE_Y; break;
        case BAIXO:    novo->body.pos.y -= STD_SIZE_Y; break;
        case ESQUERDA: novo->body.pos.x += STD_SIZE_X; break;
        case DIREITA:  novo->body.pos.x -= STD_SIZE_X; break;
    }

    novo->body.direcao = j->snake.Cauda->body.direcao;
    novo->body.color   = corpo;

    // insere no fim da lista
    j->snake.Cauda->Prox = novo;
    j->snake.Cauda = novo;   // novo passa a ser o rabo
    novo->Prox = NULL;

    j->snake.Comprimento++;
}

void IniciaBordas(Jogo *j){
    //Borda de cima
    j->bordas[0].pos = (Rectangle) {0, 0, j->LARGURA, 10};
    //Borda da direita
    j->bordas[1].pos = (Rectangle) {j->LARGURA - 10, 0, 10, j->ALTURA};
    //Borda de baixo
    j->bordas[2].pos = (Rectangle) {0, j->ALTURA - 10, j->LARGURA, 10};
    //Borda da esquerda
    j->bordas[3].pos = (Rectangle) {0, 0, 10, j->ALTURA};
}


void IniciaBarreiras1(Jogo *j){
     //Bordas do centro
    j->barreiras[0].pos = (Rectangle) {j->LARGURA- 530, j->ALTURA-490, 40, 320};
    j->barreiras[1].pos = (Rectangle) {j->LARGURA - 170, j->ALTURA-490, 40, 320};
    //Bordas verticais
    j->barreiras[2].pos = (Rectangle) {0, 0, 20, 60};
    j->barreiras[3].pos = (Rectangle) {j->LARGURA - 20, 0, 20, 60};
    j->barreiras[4].pos = (Rectangle) {0, j->ALTURA - 60, 20, 60}; 
    j->barreiras[5].pos = (Rectangle) {j->LARGURA-20, j->ALTURA - 60, 20, 60};
    //Bordas horizontais
    j->barreiras[6].pos = (Rectangle) {0, 0, j->LARGURA-600, 20};
    j->barreiras[7].pos = (Rectangle) {j->LARGURA-60, 0, j->LARGURA-600, 20};
    j->barreiras[8].pos = (Rectangle) {0, j->ALTURA - 20, 60, 20};
    j->barreiras[9].pos = (Rectangle) {j->LARGURA-60, j->ALTURA - 20, 60, 20};  
}


void IniciaFood(Jogo *j){
int colisao;
    do {
        colisao = 0; // assume que não há colisão
        j->food.pos.x = (float)((rand() % ((j->LARGURA - 20) / STD_SIZE_X)) * STD_SIZE_X + 10);
        j->food.pos.y = (float)((rand() % ((j->ALTURA - 20) / STD_SIZE_Y)) * STD_SIZE_Y + 10);
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
    FSVazia(&j->snake);
    LimparCurvas();

    IniciaSnake(j);
    IniciaBordas(j);
    IniciaFood(j);

    //j->pendingDir = CIMA;
    j->tempo = GetTime();
}



void DesenhaFood(Jogo *j){
// Desenha a comida usando a textura da png que é redimensionada para STD_SIZE_X e STD_SIZE_Y
    DrawTexturePro(
        j->tex.Food,
        (Rectangle){0, 0, j->tex.Food.width, j->tex.Food.height},           // Fonte (toda a textura)
        (Rectangle){j->food.pos.x, j->food.pos.y, STD_SIZE_X, STD_SIZE_Y}, // Destino na tela
        (Vector2){0, 0},                                            // Origem para rotação
        0,                                                           // Rotação
        WHITE                                                        // Cor
    );
}

void DesenhaBarreiras1(Jogo *j){
   
    //Desenha as barreiras do meio
    for(int i=0; i<2; i++){
        DrawTexturePro(
            j->tex.pedras,
            (Rectangle){0, 0, j->tex.pedras.width, j->tex.pedras.height},                                
            (Rectangle){j->barreiras[i].pos.x, j->barreiras[i].pos.y, j->barreiras[i].pos.width, j->barreiras[i].pos.height}, 
            (Vector2){0, 0},                                            
            0,                                                           
            WHITE                                                        
        );
    }
//Desenha as barreiras nas bordas
    for(int i = 2; i < 6; i++ ){
        DrawTexturePro(
             j->tex.pedras1,
            (Rectangle){0, 0, j->tex.pedras1.width, j->tex.pedras1.height},                   
            (Rectangle){j->barreiras[i].pos.x, j->barreiras[i].pos.y, j->barreiras[i].pos.width, j->barreiras[i].pos.height}, 
            (Vector2){0, 0},                                            
            0,                                                           
            WHITE                                                        
        ); 
    }
    for (int i= 6; i<10; i++){
        DrawTexturePro(
            j->tex.pedras2,
            (Rectangle){0, 0, j->tex.pedras2.width, j->tex.pedras2.height},                                     
            (Rectangle){j->barreiras[i].pos.x, j->barreiras[i].pos.y, j->barreiras[i].pos.width, j->barreiras[i].pos.height}, 
            (Vector2){0, 0},                                            
            0,                                                           
            WHITE                                                        
        );    
}
}

void DesenhaBarreiras3(Jogo *j) {
    DrawTexturePro(
        j->tex.tubaraoD,
        (Rectangle){
            0,
            0,
            j->tex.tubaraoD.width,
            j->tex.tubaraoD.height
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
        j->tex.tubaraoE,
        (Rectangle){
            0,
            0,
            j->tex.tubaraoE.width,
            j->tex.tubaraoE.height
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
}


void DesenhaJogo(Jogo *j){
    //DesenhaBordas(j);
    DesenhaCobra(j, j->tex.Cabeca, j->tex.Corpo, j->tex.Rabo);
    DesenhaFood(j);
}

void AtualizaPosSnake(Jogo *j){
    CelulaSnake *corpo = j->snake.Cabeca;
    while (corpo) {
        switch (corpo->body.direcao) {
            case CIMA:    corpo->body.pos.y -= STD_SIZE_Y; break;
            case BAIXO:   corpo->body.pos.y += STD_SIZE_Y; break;
            case ESQUERDA:corpo->body.pos.x -= STD_SIZE_X; break;
            case DIREITA: corpo->body.pos.x += STD_SIZE_X; break;
        }

        //aqui eu mexo no teleporte: quando a cobra chega nas bordas:
        if (corpo->body.pos.x < 0) corpo->body.pos.x = j->LARGURA - STD_SIZE_X;
        else if (corpo->body.pos.x >= j->LARGURA) corpo->body.pos.x = 0;

        if (corpo->body.pos.y < 0) corpo->body.pos.y = j->ALTURA - STD_SIZE_Y;
        else if (corpo->body.pos.y >= j->ALTURA) corpo->body.pos.y = 0;

        corpo = corpo->Prox;
    }
}

void AtualizaBarreiras3(Jogo *j){
static int iniciado = 0;

// Inicializa apenas uma vez
if (!iniciado){
// Barreira 0: direita → esquerda
j->barreiras[0].pos = (Rectangle){j->LARGURA, j->ALTURA - 530, 160, 80};
j->barreiras[0].pos = (Rectangle){j->LARGURA, j->ALTURA - 530, 160, 80};
j->barreiras[0].velocidade = -3;

// Barreira 1: esquerda → direita
j->barreiras[1].pos = (Rectangle){-j->LARGURA, j->ALTURA - 250, 160, 80};
j->barreiras[1].pos = (Rectangle){-j->LARGURA, j->ALTURA - 250, 160, 80};
j->barreiras[1].velocidade = 3;

iniciado = 1;
}

// Movimento das barreiras
for (int i = 0; i < 2; i++) {
j->barreiras[i].pos.x += j->barreiras[i].velocidade;

// Se sair pela direita, volta da esquerda
if (j->barreiras[i].pos.x > j->LARGURA) {
j->barreiras[i].pos.x = -j->barreiras[i].pos.width;
}

// Se sair pela esquerda, volta da direita
if (j->barreiras[i].pos.x < -j->barreiras[i].pos.width) {
j->barreiras[i].pos.x = j->LARGURA;
}
}
}

void AtualizaRodada(Jogo *j){
    if (GetTime() - j->tempo >= TEMPO){
        RegistrarCurvaSeDirecaoMudou(j);
        AtualizaPosSnake(j);
        ColisaoBordas(j);
        AplicarCurvasNosSegmentos(j);
        j->tempo = GetTime();
    }
}


int ColisaoFood(Jogo *j){
    if (CheckCollisionRecs(j->snake.Cabeca->body.pos, j->food.pos)){
        return 1;
    }
return 0;
}


int ColisaoBarreiras1(Jogo *j){
for(int i=0; i<10; i++){
        if (CheckCollisionRecs(j->snake.Cabeca->body.pos, j->barreiras[i].pos)){
            return 1;
        }
    }
    return 0;
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
    CelulaSnake *cab = j->snake.Cabeca;
    int dir = cab->body.direcao;

    if (cab->body.pos.x < 0) {
        AdicionaCurva(0, (int)cab->body.pos.y, dir); // curva na entrada
        cab->body.pos.x = j->LARGURA - TAMANHO_CELULA;
        AdicionaCurva((int)cab->body.pos.x, (int)cab->body.pos.y, dir); // curva na saída
    }
    else if (cab->body.pos.x >= j->LARGURA) {
        AdicionaCurva(j->LARGURA - TAMANHO_CELULA, (int)cab->body.pos.y, dir);
        cab->body.pos.x = 0;
        AdicionaCurva((int)cab->body.pos.x, (int)cab->body.pos.y, dir);
    }

    if (cab->body.pos.y < 0) {
        AdicionaCurva((int)cab->body.pos.x, 0, dir);
        cab->body.pos.y = j->ALTURA - TAMANHO_CELULA;
        AdicionaCurva((int)cab->body.pos.x, (int)cab->body.pos.y, dir);
    }
    else if (cab->body.pos.y >= j->ALTURA) {
        AdicionaCurva((int)cab->body.pos.x, j->ALTURA - TAMANHO_CELULA, dir);
        cab->body.pos.y = 0;
        AdicionaCurva((int)cab->body.pos.x, (int)cab->body.pos.y, dir);
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
        free(aux);
        
    }
    Snake->Comprimento = 0;
}

void LiberaTexturas(Jogo *j) {
    //UnloadTexture(j->tex.Cabeca);
    //UnloadTexture(j->tex.Corpo);
    //UnloadTexture(j->tex.Rabo);

    UnloadTexture(j->tex.pedras);
    UnloadTexture(j->tex.pedras1);
    UnloadTexture(j->tex.pedras2);

    UnloadTexture(j->tex.tubaraoD);
    UnloadTexture(j->tex.tubaraoE);

    UnloadTexture(j->tex.Food);
}
