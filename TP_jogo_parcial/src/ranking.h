#ifndef RANKING_H
#define RANKING_H

typedef struct{
    int pontos;
    char nome[50];
}jogador;   

//funções do ranking:
void atualizarRanking(const char *arquivo, const char *nome, int pontos);

#endif
