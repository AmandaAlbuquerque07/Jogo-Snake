#ifndef GIRACOBRA_H
#define GIRACOBRA_H

#include "raylib.h"
#include "listacobra.h"

//definindo as direções:
#define CIMA     0
#define DIREITA  1
#define BAIXO    2
#define ESQUERDA 3

//funnção que vai pegar a direção e converter em um ângulo para a imagem:
float DirecaoParaAngulo(int direcao);

//isso só pra fazer ela mexer bonitinha:
void AdicionaCurva(int x, int y, int direcao);
void RegistrarCurvaSeDirecaoMudou(Jogo *j);
void AplicarCurvasNosSegmentos(Jogo *j);
void LimparCurvas(void);


void DesenhaCobra(Jogo *j,  Texture2D texCabeca, Texture2D texCorpo, Texture2D texRabo);

#endif