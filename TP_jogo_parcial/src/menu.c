#include "raylib.h"
#include <stdio.h>
#include <string.h>

#include "menu.h"
#include "ranking.h"
#include "listacobra.h"

#define MAX_TAM 50

void desenhaIntro(Jogo*j) {
    DesenhaFundo(j, &j->tex.inicio);
    DrawText("SPEEDY", (160*j->escala), (200*j->escala), (80*j->escala), WHITE);
     DrawText("SNAKE", (190*j->escala), (300*j->escala), (80*j->escala), WHITE);
    DrawText("ENTER - Começar", (250*j->escala), (500*j->escala), (20*j->escala), GRAY);
    DrawText("ESC - Sair", (275*j->escala), (550*j->escala), (20*j->escala), GRAY);
}

void desenhaMenuPrincipal(Jogo*j) {
    ClearBackground(SKYBLUE);
    DrawText("MENU PRINCIPAL", (140*j->escala), (200*j->escala), (50*j->escala), DARKGREEN);
    DrawText("1 - Ranking", (225*j->escala), (300*j->escala), (30*j->escala), RED);
    DrawText("2 - Jogar", (225*j->escala), (350*j->escala), (30*j->escala), VIOLET);
    DrawText("ESC - Sair", (225*j->escala), (400*j->escala), (30*j->escala), DARKGRAY);
}

//TELA para escolher o tamanho das telas
void desenhaTelaTelas(Jogo* j) {
    ClearBackground(SKYBLUE);
    DrawText("Escolha o tamanho\nda sua tela de jogo", (155*j->escala), (180*j->escala), (40*j->escala), DARKGREEN);
    DrawText("1 - Pequena", (230*j->escala), (310*j->escala), (30*j->escala), DARKPURPLE);
    DrawText("2 - Grande", (230*j->escala), (360*j->escala), (30*j->escala), DARKPURPLE);
    DrawText("Pressione ESC para voltar", (190*j->escala), (450*j->escala), (20*j->escala), GRAY);
}

void desenhaTelaNome(Jogo* j, char *nomeJogador, int *tamanho) {
    ClearBackground(SKYBLUE);
    //desenha na tela o "digite seu nomme" e vai mostrando o que for digitado na string nomeJogador[MAX_TAM];

    DrawText("Digite seu nome:", (185*j->escala), (200*j->escala), (40*j->escala), DARKGREEN);
    DrawText(nomeJogador, (250*j->escala), (300*j->escala), (30*j->escala), DARKPURPLE);
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

    DrawText("Pressione ENTER para começar", (170*j->escala), (430*j->escala), (20*j->escala), GRAY);
    DrawText("Pressione ESC para voltar", (190*j->escala), (480*j->escala), (20*j->escala), GRAY);
}




void desenhaTelaRanking(Jogo*j) {
    ClearBackground(SKYBLUE);
    DrawText("RANKING", (225*j->escala), (50*j->escala), (40*j->escala), DARKPURPLE);

    //abre pra ler:
    FILE *rk = fopen("ranking.txt", "r");

    //vê se existe o ranking:
    if (rk != NULL) {
        int pontos;
        char nome[50];
        int y = (120*j->escala); //esse y detremina a posição inicial das linhas do ranking em pixels no eixo vertical
        int posicao=1; //coloca enumeração no ranking!

        while (fscanf(rk, "%d %s", &pontos, nome) == 2) { //enquanto ler o nome e os pontos com sucesso,
            char linha[120]; //esse armazena a junção dos pontos e do nome em uma linha só, pro DrawText ler, assim como o PontoNaTela, da main, que formata o ponto pro DrawText também;
            sprintf(linha, "%dº - %s : %d pts", posicao, nome, pontos); //combinando os dois E A POSIÇÃO!;
            DrawText(linha, (225*j->escala), y, (20*j->escala), RED); //texto, x, y, tam fonte, cor 
            y += (20*j->escala); //pula 20 pixels verticais pra próxima linha ficar
            posicao++;
        }
        fclose(rk);
    } else {
        DrawText("Ranking vazio!", (250*j->escala), (150*j->escala), (30*j->escala), RED);
    }

    DrawText("Pressione ENTER para voltar",(180*j->escala), (550*j->escala), (20*j->escala), GRAY);
}
