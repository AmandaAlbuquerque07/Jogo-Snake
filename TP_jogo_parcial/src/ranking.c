#include <stdio.h>
#include <string.h>
#include "ranking.h"

#define MAX_JOGADORES 20

//o que isso faz até agora? 
/*
- Lê o arquivo do ranking.
- Insere a nova pontuação na posição certinha.
- Mantém só os 20 melhores.
- Atualiza o arquivo.
*/

void atualizarRanking(const char *arquivo, const char *nome, int pontos) {
    jogador ranking[MAX_JOGADORES + 1];
    int inseridos = 0;

    // 1. Abre o arquivo pra ler
    FILE *rk = fopen(arquivo, "r");
    //abro com a+ pra ele não retornar NULL se o arquivonão existir ainda, mas sim, criá-lo;
    if (rk != NULL) {
        while (inseridos < MAX_JOGADORES &&
               fscanf(rk, "%d %s", &ranking[inseridos].pontos, ranking[inseridos].nome) == 2) {
                //por que fscanf retorna quantos itens foram lidos com sucesso
            inseridos++;
        }
        fclose(rk);
    }

    // 2. Insere a nova pontuação no vetor ranking 
    //Já ordena por inserção de uma vez:
    int i = inseridos - 1;
    //começa do fim do ranking;
    while (i >= 0 && ranking[i].pontos < pontos) {
        ranking[i + 1] = ranking[i]; // desloca para frente enquanto os valores forem menores/iguais ao que será inserido
        i--;
    }
    ranking[i + 1].pontos = pontos;//por que colocar no i+1? porque depois do while, o i+1 é o antigo i, que eu quero!
    strcpy(ranking[i + 1].nome, nome);
    inseridos++;

    if (inseridos > MAX_JOGADORES) inseridos = MAX_JOGADORES; // garante que o ranking nunca tenha mais de 20


    // 3. Atualiza o arquivo
    rk = fopen(arquivo, "w");
    if (rk != NULL) {
        for (int j = 0; j < inseridos; j++) {
            fprintf(rk, "%d %s\n", ranking[j].pontos, ranking[j].nome);
        }
        fclose(rk);
    }
}