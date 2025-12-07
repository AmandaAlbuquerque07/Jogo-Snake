#ifndef MENU_H
#define MENU_H

//enum: é uma variável que armazena 1 inteiro qualquer, que é diferente dos outros enum: marca estado;

typedef enum { 
    MENU,
    NOME,
    TELAS,
    JOGO,
    RANKING
} Estado;

// Funções do menu
void desenhaMenuPrincipal(Jogo* j);
void desenhaTelaTelas(Jogo* j);
void desenhaTelaNome(Jogo* j, char *nomeJogador, int *tamanho);
void desenhaTelaRanking(Jogo* j);

#endif
