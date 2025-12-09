#ifndef COBRA_H
#define COBRA_H

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

typedef struct Texturas{
    Texture2D Cabeca, Corpo, Rabo;
    Texture2D pedras;
    Texture2D asteroide;
    Texture2D tubaraoD ,tubaraoE;
    Texture2D Food, Food2, Food3;
    Texture2D inicio;
    Texture2D portal2;
}Texturas;

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
}ListaSnake;

typedef struct Jogo{
  ListaSnake snake;
  Food food;
  Bordas bordas[4];
  Barreiras barreiras[12];
  Texturas tex;
  double tempo;
  double cooldown;
  int LARGURA, ALTURA;
  float escala;
}Jogo;

void VSvazia(ListaSnake *snake);
void CarregaTexturas(Jogo*j);
void IniciaSnake(Jogo *j);
void AumentaSnake(Jogo *j);
void MoveSnake(Jogo *j);
int SnakeDentroDaGrade(Jogo *j);
void IniciaBordas(Jogo *j);
void IniciaBarreiras1(Jogo *j);
void IniciaBarreiras2(Jogo *j);
void Barreiras3(Jogo *j);
void IniciaPorta1(Jogo *j);
void IniciaPorta2(Jogo *j);
void IniciaFood(Jogo *j);
void IniciaJogo(Jogo *j);
void DesenhaFood(Jogo *j); 
void DesenhaFood2(Jogo *j);
void DesenhaFood3(Jogo *j);
void DesenhaBordas(Jogo *j);
void DesenhaBarreiras1(Jogo *j);
void DesenhaBarreiras2(Jogo *j);
void DesenhaBarreiras3(Jogo *j);
void DesenhaPorta1(Jogo *j);
void DesenhaPorta2(Jogo *j);
void DesenhaJogo(Jogo *j);
void DesenhaJogo2(Jogo *j);
void DesenhaJogo3(Jogo *j);
void DesenhaFundo(Jogo *j, Texture2D* img);
void AtualizaRodada(Jogo *j);
void AtualizaBarreiras2(Jogo *j);
int ColisaoFood(Jogo *j);
void ColisaoBordas(Jogo *j);
int ColisaoBarreiras1(Jogo *j);
int ColisaoBarreiras2(Jogo *j);
int ColisaoBarreiras3(Jogo* j);
int ColisaoPorta1(Jogo *j);
int ColisaoPorta2(Jogo *j);
int ColisaoSnake(Jogo *j);
void FreeLista(ListaSnake *Snake);
void LiberaTexturas(Jogo *j);

#endif
