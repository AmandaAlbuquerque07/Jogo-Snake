#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "listacobra.h"
#include "giracobra.h"
#define TAMANHO_CELULA 40
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
    j->tex.asteroide = LoadTexture("Assets/AsteroideFrog.png");
    j->tex.tubaraoD  = LoadTexture("Assets/tubarao2.png");
    j->tex.tubaraoE  = LoadTexture("Assets/tubarao1.png");
    j->tex.Food = LoadTexture("Assets/maca.png");
    j->tex.Food2 = LoadTexture("Assets/estrela.png");
    j->tex.Food3 = LoadTexture("Assets/peixe.png");
    j->tex.inicio= LoadTexture("Assets/inicio.jpg");
}

void IniciaSnake(Jogo *j){
    j->snake.Cabeca = (SnakeApontador)malloc(sizeof(CelulaSnake));
    memset(j->snake.Cabeca, 0, sizeof(CelulaSnake));
    j->snake.Cauda  = (SnakeApontador)malloc(sizeof(CelulaSnake));
    memset(j->snake.Cauda, 0, sizeof(CelulaSnake));

    j->snake.Cabeca->Prox = j->snake.Cauda;
    j->snake.Cauda->Prox  = NULL;

    j->snake.Cabeca->body.pos = (Rectangle){ j->LARGURA/2, j->ALTURA-90  , STD_SIZE_X, STD_SIZE_Y };
    j->snake.Cabeca->body.direcao = CIMA;
    j->snake.Cabeca->body.color   = j->tex.Cabeca;

    j->snake.Cauda->body.pos = (Rectangle){ j->snake.Cabeca->body.pos.x, j->snake.Cabeca->body.pos.y + STD_SIZE_Y, STD_SIZE_X, STD_SIZE_Y };
    j->snake.Cauda->body.direcao = CIMA;
    j->snake.Cauda->body.color   = j->tex.Rabo;

    j->snake.Comprimento = 2;
}

void AumentaSnake(Jogo *j){
    SnakeApontador novo  = malloc(sizeof(CelulaSnake));
    memset(novo, 0, sizeof(CelulaSnake));

    novo->body.pos = j->snake.Cauda->body.pos;
    switch (j->snake.Cauda->body.direcao) {
        case CIMA:     novo->body.pos.y += STD_SIZE_Y; break;
        case BAIXO:    novo->body.pos.y -= STD_SIZE_Y; break;
        case ESQUERDA: novo->body.pos.x += STD_SIZE_X; break;
        case DIREITA:  novo->body.pos.x -= STD_SIZE_X; break;
    }

    novo->body.direcao = j->snake.Cauda->body.direcao;
    novo->body.color   = j->tex.Corpo;

    //insere no fim da lista
    j->snake.Cauda->Prox = novo;
    j->snake.Cauda = novo;   //novo passa a ser o rabo, pra manter a textura
    novo->Prox = NULL;
    j->snake.Comprimento++;
}

void MoveSnake(Jogo *j) {
    //começa na cabeça
    SnakeApontador atual = j->snake.Cabeca;

    //guarda a posição anterior da cabeça
    Rectangle posAnterior = atual->body.pos;

    //move a cabeça primeiro (40 em 40)
    switch (atual->body.direcao) {
        case CIMA:
            atual->body.pos.y -= 40;
            break;
        case BAIXO:
            atual->body.pos.y += 40;
            break;
        case ESQUERDA:
            atual->body.pos.x -= 40;
            break;
        case DIREITA:
            atual->body.pos.x += 40;
            break;
    }

    // trata bordas depois de mover
    ColisaoBordas(j);

    //agora o corpo segue a posição anterior
    atual = atual->Prox;

    while (atual != NULL) {
        Rectangle temp = atual->body.pos;
        atual->body.pos = posAnterior;
        posAnterior = temp;
        atual = atual->Prox;
    }
}
 
int SnakeDentroDaGrade(Jogo *j) {
    float x = j->snake.Cabeca->body.pos.x;
    float y = j->snake.Cabeca->body.pos.y;

    return (x >= 10 &&
            x <= j->LARGURA - 10 - TAMANHO_CELULA &&
            y >= 10 &&
            y <= j->ALTURA  - 10 - TAMANHO_CELULA);
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
    // Barreiras do centro (duas colunas altas)
    if(j->LARGURA==660){
        j->barreiras[0].pos = (Rectangle) {j->ALTURA-530, j->ALTURA-490, 40, 320};
        j->barreiras[1].pos = (Rectangle) {j->LARGURA-170, j->ALTURA-490, 40, 320};
    }else{
        j->barreiras[0].pos = (Rectangle) {j->LARGURA-650, j->ALTURA-610, 80, 400};
        j->barreiras[1].pos = (Rectangle) {j->LARGURA-290, j->ALTURA-610, 80, 400};
    }
}

void IniciaBarreiras2(Jogo *j){
    if (ColisaoBarreiras2(j) || ColisaoFood(j)) {
        AtualizaBarreiras2(j);
    }
    else {
        for (int i = 0; i < 3; i++) {
            j->barreiras[i].pos.width  = 120; // 3 blocos
            j->barreiras[i].pos.height = 120; 
        }

        j->barreiras[0].pos.x = j->LARGURA - (170 * j->escala);
        j->barreiras[0].pos.y = j->ALTURA  - (410 * j->escala);
        
        j->barreiras[1].pos.x = j->LARGURA - (570 * j->escala);
        j->barreiras[1].pos.y = j->ALTURA  - (170 * j->escala);

        j->barreiras[2].pos.x = j->LARGURA - (290 * j->escala);
        j->barreiras[2].pos.y = j->ALTURA  - (170 * j->escala);
    }
}

void Barreiras3(Jogo *j){
    static int iniciado = 0;
    // Inicializa apenas uma vez
        if (!iniciado){
        // Barreira 0: direita pra esquerda
        j->barreiras[0].pos = (Rectangle){j->LARGURA, (j->ALTURA - 530*j->escala), (160*j->escala), (80*j->escala)};
        j->barreiras[0].velocidade = -3;

        // Barreira 1: esquerda pra direita
        j->barreiras[1].pos = (Rectangle){-j->LARGURA, (j->ALTURA - 250*j->escala), (160*j->escala), (80*j->escala)};
        j->barreiras[1].velocidade = 3;

        iniciado = 1;
    }


    // Movimento das barreiras:
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

void IniciaPorta1(Jogo *j){
    j->barreiras[10].pos = (Rectangle) {
        (610),
        (90),
        (40*j->escala),
        (40*j->escala)
    };  
}

void IniciaPorta2(Jogo *j){
    j->barreiras[11].pos = (Rectangle) {
        (570),
        (570),
        (40*j->escala),
        (40*j->escala)
    };  
}

void IniciaPorta3(Jogo *j){
    j->barreiras[12].pos = (Rectangle) {
        (90),
        (90),
        (40*j->escala),
        (40*j->escala)
    };  
}

void DesenhaFundo(Jogo *j, Texture2D* img){

    DrawTexturePro(
        *img,
        (Rectangle){0, 0, img->width, img->height},          
        (Rectangle){0, 0, (j->LARGURA*j->escala), (j->ALTURA*j->escala)}, 
        (Vector2){0, 0},                                          
        0,                                                          
        WHITE                                                     
    );
}

void IniciaFood(Jogo *j){

    int colisao;
    do {
            colisao = 0; // assume que não há colisão
            j->food.pos.x = (float)((rand() % ((j->LARGURA - 20) / STD_SIZE_X)) * STD_SIZE_X + 10);
            j->food.pos.y = (float)((rand() % ((j->ALTURA-20) / STD_SIZE_Y)) * STD_SIZE_Y + 10);
            j->food.pos.width = STD_SIZE_X;
            j->food.pos.height = STD_SIZE_Y;

            //verifica se a nova posição colide com a cobra
            SnakeApontador aux = j->snake.Cabeca;

            while(aux != NULL){
                if(CheckCollisionRecs(j->food.pos, aux->body.pos)){
                    colisao = 1; //houve colisão, precisa gerar outra posição
                    break;
                }
            aux = aux->Prox;
            } 
            
                for(int i=0; i<10; i++){
                    if(CheckCollisionRecs(j->food.pos, j->barreiras[i].pos)){
                        colisao = 1; //houve colisão, precisa gerar outra posição
                        break;
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

    j->tempo = GetTime();
}



void DesenhaFood(Jogo *j){
//Desenha a comida usando a textura da png que é redimensionada para STD_SIZE_X e STD_SIZE_Y
    DrawTexturePro(
    j->tex.Food,
    (Rectangle){0, 0, j->tex.Food.width, j->tex.Food.height},           // Fonte (toda a textura)
    (Rectangle){j->food.pos.x, j->food.pos.y, STD_SIZE_X, STD_SIZE_Y},  // Destino na tela
    (Vector2){0, 0},                                                    // Origem para rotação
    0,                                                                  // Rotação
    WHITE                                                               // Cor
    );
}

void DesenhaFood2(Jogo *j){
    DrawTexturePro(
        j->tex.Food2,
        (Rectangle){0, 0, j->tex.Food2.width, j->tex.Food2.height},          
        (Rectangle){j->food.pos.x, j->food.pos.y, STD_SIZE_X, STD_SIZE_Y}, 
        (Vector2){0, 0},                                           
        0,                                                         
        WHITE                                                      
    );
}

void DesenhaFood3(Jogo *j){
DrawTexturePro(
        j->tex.Food3,
        (Rectangle){0, 0, j->tex.Food3.width, j->tex.Food3.height},        
        (Rectangle){j->food.pos.x, j->food.pos.y, STD_SIZE_X, STD_SIZE_Y}, 
        (Vector2){0, 0},                                                   
        0,                                                                 
        WHITE                                                              
    );
}

void DesenhaBarreiras1(Jogo *j){
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
}

void DesenhaBarreiras2(Jogo *j){
    for (int i = 0; i < 3; i++) {
        DrawTexturePro(
            j->tex.asteroide,
            (Rectangle){0, 0, j->tex.asteroide.width, j->tex.asteroide.height},
            j->barreiras[i].pos,   //USANDO DIRETO O RECTANGLE DA COLISÃO p evitar o bug invisível
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
        }, 

        (Rectangle){
            j->barreiras[0].pos.x,
            j->barreiras[0].pos.y,
            160,      
            80
        }, 

        (Vector2){0, 0},   
            0.0f,         
            WHITE
    );

    DrawTexturePro(
        j->tex.tubaraoE,
        (Rectangle){
            0,
            0,
            j->tex.tubaraoE.width,
            j->tex.tubaraoE.height
        },

        (Rectangle){
            j->barreiras[1].pos.x,
            j->barreiras[1].pos.y,
            160,      
            80
        }, 

        (Vector2){0, 0},   
            0.0f,          
            WHITE
    );   
}

void DesenhaPorta1(Jogo *j){
        DrawTexturePro(
        j->tex.Food2,
        (Rectangle){0, 0, j->tex.Food2.width, j->tex.Food2.height},                                
        (Rectangle){j->barreiras[10].pos.x, j->barreiras[10].pos.y, j->barreiras[10].pos.width, j->barreiras[10].pos.height}, 
        (Vector2){0, 0},                                            
        0,                                                           
        WHITE                                                        
    );
}

void DesenhaPorta2(Jogo *j){
        DrawTexturePro(
        j->tex.Food3,
        (Rectangle){0, 0, j->tex.Food3.width, j->tex.Food3.height},                                
        (Rectangle){j->barreiras[11].pos.x, j->barreiras[11].pos.y, j->barreiras[11].pos.width, j->barreiras[11].pos.height}, 
        (Vector2){0, 0},                                            
        0,                                                           
        WHITE                                                        
    );
}

void DesenhaPorta3(Jogo *j){
        DrawTexturePro(
        j->tex.Food,
        (Rectangle){0, 0, j->tex.Food.width, j->tex.Food.height},                                
        (Rectangle){j->barreiras[12].pos.x, j->barreiras[12].pos.y, j->barreiras[12].pos.width, j->barreiras[12].pos.height}, 
        (Vector2){0, 0},                                            
        0,                                                           
        WHITE                                                        
    );
}


void DesenhaJogo(Jogo *j){
    DesenhaCobra(j);
    DesenhaFood(j);
}
void DesenhaJogo2(Jogo *j){
    DesenhaCobra(j);
    DesenhaFood2(j);
}
void DesenhaJogo3(Jogo *j){
    DesenhaCobra(j);
    DesenhaFood3(j);
}


void AtualizaRodada(Jogo *j){
    if (GetTime() - j->tempo >= TEMPO){
        RegistrarCurvaSeDirecaoMudou(j);
        MoveSnake(j);
        ColisaoBordas(j);
        AplicarCurvasNosSegmentos(j);
        j->tempo = GetTime();
    }
}

void AtualizaBarreiras2(Jogo *j){
    int colisao;
    float distanciaMin = 160.0f; //4 blocos de 40

    //tamanho fixo que combina com o desenho (3 quaddradinhos)
    for (int i = 0; i < 3; i++) {
        j->barreiras[i].pos.width  = 120;
        j->barreiras[i].pos.height = 120;
    }

    do {
        colisao = 0;

        //sorteia posições
        for (int i = 0; i < 3; i++) {

            int colunas = (j->LARGURA - 20 - 120) / 40;
            int linhas  = (j->ALTURA  - 20 - 120) / 40;

            int cx = GetRandomValue(0, colunas);
            int cy = GetRandomValue(0, linhas);

            j->barreiras[i].pos.x = 10 + cx * 40;
            j->barreiras[i].pos.y = 10 + cy * 40;
        }

        //distância mínima entre as barreiras
        for (int i = 0; i < 3 && !colisao; i++) {
            for (int k = i + 1; k < 3; k++) {
                float dx = j->barreiras[k].pos.x - j->barreiras[i].pos.x;
                float dy = j->barreiras[k].pos.y - j->barreiras[i].pos.y;
                float dist = sqrt(dx*dx + dy*dy);

                if (dist < distanciaMin) {
                    colisao = 1;
                }
            }
        }

        //colisão com a comida
        if (!colisao) {
            for (int i = 0; i < 3; i++) {
                if (CheckCollisionRecs(j->barreiras[i].pos, j->food.pos)) {
                    colisao = 1;
                    break;
                }
            }
        }

        //colisão com a cobra
        if (!colisao) {
            SnakeApontador aux = j->snake.Cabeca;

            while (aux != NULL && !colisao) {
                for (int i = 0; i < 3; i++) {
                    if (CheckCollisionRecs(j->barreiras[i].pos, aux->body.pos)) {
                        colisao = 1;
                        break;
                    }
                }
                aux = aux->Prox;
            }
        }

    } while (colisao);
}


int ColisaoFood(Jogo *j){
    if (CheckCollisionRecs(j->snake.Cabeca->body.pos, j->food.pos)){
        return 1;
    }
return 0;
}


int ColisaoBarreiras1(Jogo *j){
    for(int i=0; i<2; i++){
        if (CheckCollisionRecs(j->snake.Cabeca->body.pos, j->barreiras[i].pos)){
            return 1;
        }
    }
    return 0;
}

int ColisaoBarreiras2(Jogo *j){
    SnakeApontador cabeca = j->snake.Cabeca;
    SnakeApontador aux = j->snake.Cabeca->Prox;   //começa no PRIMEIRO DO CORPO

    //verifica colisão da CABEÇA
    for (int i = 0; i < 3; i++){
        if (CheckCollisionRecs(cabeca->body.pos, j->barreiras[i].pos)){
            return 1;   // cabeça bateu → morre
        }
    }

    //verifica colisão do CORPO
    while (aux != NULL){
        for (int i = 0; i < 3; i++){
            if (CheckCollisionRecs(aux->body.pos, j->barreiras[i].pos)){
                return 2;   //corpo bateu
            }
        }
        aux = aux->Prox;
    }

    return 0; //nada colidiu
}

int ColisaoBarreiras3(Jogo *j){
    SnakeApontador aux = j->snake.Cabeca;

    //Passa por TODOS os segmentos da cobra
    while (aux != NULL){
        for (int i = 0; i < 2; i++){    //as 2 barreiras móveis
            if (CheckCollisionRecs(aux->body.pos, j->barreiras[i].pos)){
                return 1;   //bateu, morre
            }
        }
    aux = aux->Prox;
    }
    return 0; // Não colidiu
}

/*Explicando os portais: o ícone diferente de comidinha te leva
a passar de mapa*/
int ColisaoPorta1(Jogo *j){
    if (!j->snake.Cabeca) return 0;

    return CheckCollisionRecs(
        j->snake.Cabeca->body.pos,
        j->barreiras[10].pos
    );
}

int ColisaoPorta2(Jogo *j){
    if (!j->snake.Cabeca) return 0;

    return CheckCollisionRecs(
        j->snake.Cabeca->body.pos,
        j->barreiras[11].pos
    );
}

int ColisaoPorta3(Jogo *j){
    if (!j->snake.Cabeca) return 0;

    return CheckCollisionRecs(
        j->snake.Cabeca->body.pos,
        j->barreiras[12].pos
    );
}

void ColisaoBordas(Jogo *j) {
    CelulaSnake *cab = j->snake.Cabeca;

    int min = 10;

    int colunas = (j->LARGURA - 20) / TAMANHO_CELULA;
    int linhas  = (j->ALTURA  - 20) / TAMANHO_CELULA;

    int maxX = min + (colunas - 1) * TAMANHO_CELULA;
    int maxY = min + (linhas  - 1) * TAMANHO_CELULA;

    //no eixo x
    if (cab->body.pos.x < min)
        cab->body.pos.x = maxX;
    else if (cab->body.pos.x > maxX)
        cab->body.pos.x = min;

    //no eixo y
    if (cab->body.pos.y < min)
        cab->body.pos.y = maxY;
    else if (cab->body.pos.y > maxY)
        cab->body.pos.y = min;
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
    UnloadTexture(j->tex.Cabeca);
    UnloadTexture(j->tex.Corpo);
    UnloadTexture(j->tex.Rabo);
    UnloadTexture(j->tex.pedras);
    UnloadTexture(j->tex.asteroide);
    UnloadTexture(j->tex.tubaraoD);
    UnloadTexture(j->tex.tubaraoE);
    UnloadTexture(j->tex.Food);
    UnloadTexture(j->tex.Food2);
    UnloadTexture(j->tex.Food3);
    UnloadTexture(j->tex.inicio);
}
