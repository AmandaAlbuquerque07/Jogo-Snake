#ifndef MENU_H
#define MENU_H

#include "listacobra.h"

//enum: é uma variável que armazena 1 inteiro qualquer, que é diferente dos outros enum: marca estado;

typedef enum { 
    INTRO,
    MENU,
    NOME,
    TELAS,
    JOGO,
    RANKING
} Estado;

// Funções do menu
void desenhaIntro(Jogo*j);
void desenhaMenuPrincipal(Jogo* j);
void desenhaTelaTelas(Jogo* j);
void desenhaTelaNome(Jogo* j, char *nomeJogador, int *tamanho);
void desenhaTelaRanking(Jogo* j);

#endif
