#ifndef COBRA_H
#define COBRA_H

#define LARGURA 660
#define ALTURA 660
#define STD_SIZE_X 40
#define STD_SIZE_Y 40
#define TEMPO 0.16
#define COOLDOWN 0.2
#define SNAKE_COLOR VIOLET
#define FOOD_COLOR RED

typedef struct Bordas{
    Rectangle pos;
}Bordas;

typedef struct Food{
    Rectangle pos;
    Color color;
}Food;

typedef struct Barreiras{
    Rectangle pos;
    Color color;
    int velocidade;
    int inicia;
}Barreiras;

//Início da Implementação da lista:

typedef struct Body{
    Rectangle pos;
    Texture2D color;
    int direcao;
}Body; //TipoItem

typedef struct CelulaSnake * SnakeApontador;

typedef struct CelulaSnake {
    Body body;
    SnakeApontador Prox;
} CelulaSnake; //TipoCelula

typedef struct {
    SnakeApontador Cabeca, Cauda;
    int Comprimento;
    int dirX, dirY;
}ListaSnake;

typedef struct Jogo{
    ListaSnake snake;
    Food food;
    Bordas bordas[4];
    Barreiras barreiras[10];
    double tempo;
    double cooldown;
}Jogo;

void VSvazia(ListaSnake *snake);
void IniciaSnake(Jogo *j);
void AumentaSnake(Jogo *j);
void IniciaBordas(Jogo *j);
void IniciaBarreiras1(Jogo *j);
void IniciaFood(Jogo *j);
void IniciaJogo(Jogo *j);
void DesenhaSnake(Jogo *j);
void DesenhaFood(Jogo *j, Texture2D maca); //Desenha uma comida em uma posição aleatória
void DesenhaBordas(Jogo *j);
void DesenhaBarreiras1(Jogo *j, Texture2D pedras, Texture2D pedras1, Texture2D pedras2);
void DesenhaBarreiras3(Jogo *j);
void DesenhaJogo(Jogo *j, Texture2D maca);
void AtualizaDirecao(Jogo *j); //Atualiza a direção da cobrinha
void AtualizaPosSnake(Jogo *j); //Atualiza a posição da cobrinha
void AtualizaBarreiras3(Jogo *j);
void AtualizaRodada(Jogo *j);
void DesenhaFood(Jogo *j, Texture2D maca);
int ColisaoFood(Jogo *j);
void ColisaoBordas(Jogo *j);
int ColisaoBarreiras1(Jogo *j);
int ColisaoBarreiras3(Jogo* j);
int ColisaoSnake(Jogo *j);
void FreeLista(ListaSnake *Snake);

#endif
