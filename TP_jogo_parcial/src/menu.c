#include "raylib.h"
#include <stdio.h>
#include <string.h>

#include "menu.h"
#include "ranking.h"

#define MAX_TAM 50

void desenhaMenuPrincipal() {
    ClearBackground(SKYBLUE);
    DrawText("MENU PRINCIPAL", 165, 200, 40, BLACK);
    DrawText("1 - Ranking", 225, 300, 30, RED);
    DrawText("2 - Jogar", 225, 350, 30, VIOLET);
    DrawText("ESC - Sair", 225, 400, 30, DARKGRAY);
}


void desenhaTelaNome(char *nomeJogador, int *tamanho) {
    ClearBackground(SKYBLUE);
    //desenha na tela o "digite seu nomme" e vai mostrando o que for digitado na string nomeJogador[MAX_TAM];

    DrawText("Digite seu nome:", 215, 100, 30, BLACK);
    DrawText(nomeJogador, 250, 150, 30, VIOLET);

    //Isso aqui é o loop de coleta das letras a partir da pressão nas teclas:
    int tecla = GetCharPressed();
    while (tecla > 0) {
        if ((tecla >= 32) && (tecla <= 125) && (*tamanho < MAX_TAM-1)) {
            //porque eu quero só letras e porque tem que ter espaço pro \0 ^ ;
            nomeJogador[*tamanho] = (char)tecla;
            (*tamanho)++;
            //vou andando na string ^ ;
            nomeJogador[*tamanho] = '\0';
        }
        tecla = GetCharPressed();
    }

    //isso aqui permite que a pessoa apague também:
    if (IsKeyPressed(KEY_BACKSPACE) && *tamanho > 0) {
        (*tamanho)--;
        nomeJogador[*tamanho] = '\0';
    }

    DrawText("Pressione ENTER para começar", 175, 400, 20, GRAY);
}




void desenhaTelaRanking() {
    ClearBackground(SKYBLUE);
    DrawText("RANKING", 250, 50, 40, BLACK);

    //abre pra ler:
    FILE *rk = fopen("ranking.txt", "r");

    //vê se existe o ranking:
    if (rk != NULL) {
        int pontos;
        char nome[50];
        int y = 120; //esse y detremina a posição inicial das linhas do ranking em pixels no eixo vertical
        int posicao=1; //coloca enumeração no ranking!

        while (fscanf(rk, "%d %s", &pontos, nome) == 2) { //enquanto ler o nome e os pontos com sucesso,
            char linha[120]; //esse armazena a junção dos pontos e do nome em uma linha só, pro DrawText ler, assim como o PontoNaTela, da main, que formata o ponto pro DrawText também;
            sprintf(linha, "%dº - %s : %d pts", posicao, nome, pontos); //combinando os dois E A POSIÇÃO!;
            DrawText(linha, 225, y, 20, RED); //texto, x, y, tam fonte, cor 
            y += 20; //pula 20 pixels verticais pra próxima linha ficar
            posicao++;
        }
        fclose(rk);
    } else {
        DrawText("Ranking vazio!", 250, 150, 30, RED);
    }

    DrawText("Pressione ENTER para voltar", 200, 550, 20, GRAY);
}


