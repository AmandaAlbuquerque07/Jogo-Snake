#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "listacobra.h"

#include "ranking.h"
#include "menu.h"

int Pontos;
char Nome[50];

int main(){
    Jogo jogo;
    int gameOver = 1;
    Pontos=0;
    char PontoNaTela[20];
    int tamanhoNome=0;


    //Cria a janela do jogo;
    InitWindow(LARGURA, ALTURA, "Snake Game");
    SetTargetFPS(50);
    srand(time(NULL));

    Texture2D maca = LoadTexture("Assets/maca.png"); // carrega as imagens
    Texture2D fundo1 = LoadTexture("Assets/GramaFundo.jpeg");
    Texture2D fundo2 = LoadTexture("Assets/espaco.png");


    //a partir daqui tudo novo:
    Estado estado= MENU;
     while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (estado) { //determina o que fazer em cada estado do jogo:
            case MENU:
                desenhaMenuPrincipal();
                if (IsKeyPressed(KEY_ONE)) estado = RANKING; //se apertar 1 abre o ranking,
                if (IsKeyPressed(KEY_TWO)) estado = NOME; //2 -> JOGAR! Pede o NOME antes! Por isso tem o caso NOME.
                if (IsKeyPressed(KEY_ESCAPE)) CloseWindow();//fecha a tela com esc;
                break;

            case NOME:
                desenhaTelaNome(Nome, &tamanhoNome); //recebe o nome.
                if (IsKeyPressed(KEY_ENTER) && tamanhoNome > 0) { //começa o jogo!
                    IniciaJogo(&jogo);
                    Pontos = 0;
                    gameOver = 1;
                    estado = JOGO;
                }
                break;

            case JOGO: // o que fazer no jogo? todas as funções que já tínhamos:
            if(Pontos <= 2){
                DrawTexture(fundo1, 0, 0, WHITE);
                if (gameOver) {
                    DesenhaJogo(&jogo, maca);
                    AtualizaRodada(&jogo);

                    if (ColisaoFood(&jogo)) {
                        IniciaFood(&jogo);
                        AumentaSnake(&jogo);
                        Pontos++; //atualiza pontuação
                    }

                    //mostra pontuação:
                    sprintf(PontoNaTela, "Score: %d", Pontos);
                    DrawText(PontoNaTela, 10, 10, 30, WHITE);

                    if (ColisaoBordas(&jogo) || ColisaoSnake(&jogo)) {
                        gameOver = 0;
                    }
                }else{
                     atualizarRanking("ranking.txt", Nome, Pontos);
                    DrawText("FIM DE JOGO", 200, 200, 40, RED);
                    DrawText("Pressione Enter para voltar ao menu", 110, 350, 25, WHITE); //texto, x, y, tam fonte, cor
                    if (IsKeyPressed(KEY_ENTER)) {                                      
                        estado = MENU;
                        Nome[0] = '\0'; // limpa o nome pra próxima partida!
                        tamanhoNome = 0;
                    }
                } 

            }else if(Pontos > 2){

                DrawTexture(fundo2, 0, 0, WHITE);
                if (gameOver) {
                    DesenhaJogo(&jogo, maca);
                    AtualizaRodada(&jogo);

                    if (ColisaoFood(&jogo)) {
                        IniciaFood(&jogo);
                        AumentaSnake(&jogo);
                        Pontos++; //atualiza pontuação
                    }

                    //mostra pontuação:
                    sprintf(PontoNaTela, "Score: %d", Pontos);
                    DrawText(PontoNaTela, 10, 10, 30, WHITE);

                    if (ColisaoBordas(&jogo) || ColisaoSnake(&jogo)) {
                        gameOver = 0;
                    }
                } else { //quando o jogador perde:
                    atualizarRanking("ranking.txt", Nome, Pontos);
                    DrawText("FIM DE JOGO", 200, 200, 40, RED);
                    DrawText("Pressione Enter para voltar ao menu", 110, 350, 25, WHITE); //texto, x, y, tam fonte, cor
                    if (IsKeyPressed(KEY_ENTER)) {                                      
                        estado = MENU;
                        Nome[0] = '\0'; // limpa o nome pra próxima partida!
                        tamanhoNome = 0;
                    }
                }
                break;

            case RANKING:
                desenhaTelaRanking();
                if (IsKeyPressed(KEY_ESCAPE)) estado = MENU;
                break;
        }
    }

        EndDrawing();
    }


    UnloadTexture(maca); // libera as texturas
    UnloadTexture(fundo1);
    UnloadTexture(fundo2);
    FreeLista(&jogo.snake); 
    CloseWindow();
    return 0;
}
