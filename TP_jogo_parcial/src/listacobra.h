#ifndef COBRA_H
#define COBRA_H

#define LARGURA 660
#define ALTURA 660
#define STD_SIZE_X 40
#define STD_SIZE_Y 40
#define TEMPO 0.2
#define COOLDOWN 0.2
#define SNAKE_COLOR YELLOW
#define FOOD_COLOR BLUE

typedef struct Bordas{
    Rectangle pos;
}Bordas;

typedef struct Body{
    Rectangle pos;
    Color color;
    int direcao;
}Body;

typedef struct Food{
    Rectangle pos;
    Color color;
}Food;

typedef struct Jogo{
    Body body;
    Food food;
    Bordas bordas[4];
    double tempo;
    double cooldown;
}Jogo;


void IniciaBody(Jogo *j);
void IniciaBordas(Jogo *j);
void IniciaFood(Jogo *j);
void IniciaJogo(Jogo *j);
void DesenhaBody(Jogo *j); 
void DesenhaFood(Jogo *j); //Desenha uma comida em uma posição aleatória
void DesenhaBordas(Jogo *j);
void DesenhaJogo(Jogo *j);
void AtualizaDirecao(Jogo *j); //Atualiza a direção da cobrinha
void AtualizaPosBody(Jogo *j); //Atualiza a posição da cobrinha
void AtualizaRodada(Jogo *j);
int ColisaoFood(Jogo *j);
int ColisaoBordas(Jogo *j);


#endif