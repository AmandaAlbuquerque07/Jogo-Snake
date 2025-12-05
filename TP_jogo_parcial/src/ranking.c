  #include <stdio.h>
    #include <string.h>
    #include "ranking.h"

    #define MAX_JOGADORES 20

    void atualizarRanking(const char *arquivo, const char *nome, int pontos) {
        jogador ranking[MAX_JOGADORES + 1];
        int inseridos = 0;

        // 1. Abre o arquivo pra ler
        FILE *rk = fopen(arquivo, "r");
        if (rk) {
            while (inseridos < MAX_JOGADORES &&
                fscanf(rk, "%d %49s", &ranking[inseridos].pontos, ranking[inseridos].nome) == 2) {
                    //por que fscanf retorna quantos itens foram lidos com sucesso
                inseridos++;
            }
            fclose(rk);
        }

        // 2) Adicionar novo jogador no fim
        ranking[inseridos].pontos = pontos;
        strncpy(ranking[inseridos].nome, nome, sizeof(ranking[inseridos].nome) - 1);
        ranking[inseridos].nome[sizeof(ranking[inseridos].nome) - 1] = '\0';
        inseridos++;

        // 3) Ordenar por pontos (maior primeiro)
        for (int i = 0; i < inseridos - 1; i++) {
            for (int j = i + 1; j < inseridos; j++) {
                if (ranking[j].pontos > ranking[i].pontos) {
                   jogador tmp = ranking[i];
                    ranking[i] = ranking[j];
                 ranking[j] = tmp;
                }
            }
        }

        if (inseridos > MAX_JOGADORES) inseridos = MAX_JOGADORES; // garante que o ranking nunca tenha mais de 20

        // 4) Atualiza o arquivo
        rk = fopen(arquivo, "w");
        if (rk) {
            for (int j = 0; j < inseridos; j++) {
                fprintf(rk, "%d %s\n", ranking[j].pontos, ranking[j].nome);
            }
            fclose(rk);
        }
    }
