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

    InitAudioDevice();   // inicializa o sistema de som
    Music trilha = LoadMusicStream("Assets/trilha2.mp3");
    SetMusicVolume(trilha, 0.5f);
    PlayMusicStream(trilha);

    SetExitKey(KEY_NULL);//pra não dar o bug do esc sempre fechar o jogo.
    SetTargetFPS(50);
    srand(time(NULL));

    Sound somComer = LoadSound("Assets/somComer.mp3");
    Sound somMorrer1 = LoadSound("Assets/somMorrer1.mp3");
    Sound somMorrer2 = LoadSound("Assets/somMorrer2.mp3");
    Sound somMorrer3 = LoadSound("Assets/somMorrer3.mp3");

    Texture2D maca = LoadTexture("Assets/maca.png"); // carrega as imagens
    Texture2D fundo1 = LoadTexture("Assets/GramaFundo.jpeg");
    Texture2D fundo2 = LoadTexture("Assets/espaco.png");
    Texture2D fundo3 = LoadTexture("Assets/fundomar.jpeg");

    //a partir daqui tudo novo:
    Estado estado= MENU;
        while (!WindowShouldClose()) {
        UpdateMusicStream(trilha);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (estado) { //determina o que fazer em cada estado do jogo:
            case MENU:
                desenhaMenuPrincipal();
                if (IsKeyPressed(KEY_ONE)) estado = RANKING; //se apertar 1 abre o ranking,
                if (IsKeyPressed(KEY_TWO)) estado = NOME; //2 -> JOGAR! Pede o NOME antes! Por isso tem o caso NOME.
                if (IsKeyPressed(KEY_ESCAPE)){
                    CloseWindow();//fecha a tela com esc;
                } 
                break;

            case NOME:
                desenhaTelaNome(Nome, &tamanhoNome); //recebe o nome.
                if (IsKeyPressed(KEY_ENTER) && tamanhoNome > 0) { //começa o jogo!
                    IniciaJogo(&jogo);
                    Pontos = 0;
                    gameOver = 1;
                    estado = JOGO;
                }
                if(IsKeyPressed(KEY_ESCAPE)){
                    estado=MENU;
                    Nome[0]='\0';
                    tamanhoNome=0;
                }
                break;
                

            case JOGO: // o que fazer no jogo? todas as funções que já tínhamos:

                if(Pontos <= 2){

                    DrawTexture(fundo1, 0, 0, WHITE);
                    if (gameOver) {
                        DesenhaJogo(&jogo, maca);
                        AtualizaRodada(&jogo);

                        if (ColisaoFood(&jogo)) {
                            PlaySound(somComer);
                            IniciaFood(&jogo);
                            AumentaSnake(&jogo);
                            Pontos++; //atualiza pontuação
                        }

                        //mostra pontuação:
                        sprintf(PontoNaTela, "Score: %d", Pontos);
                        DrawText(PontoNaTela, 10, 10, 30, WHITE);

                        ColisaoBordas(&jogo);
                        if (ColisaoSnake(&jogo)) {
                            PlaySound(somMorrer1);
                            gameOver = 0;
                        }

                        }else{
                        DrawText("FIM DE JOGO", 150, 200, 60, RED);
                        DrawText("Pressione Enter para voltar ao menu", 110, 350, 25, WHITE); //texto, x, y, tam fonte, cor
                        if (IsKeyPressed(KEY_ENTER)) { 
                            atualizarRanking("ranking.txt", Nome, Pontos);                                     
                            estado = MENU;
                            Nome[0] = '\0'; // limpa o nome pra próxima partida!
                            tamanhoNome = 0;
                        }
                    } 

                }else if(Pontos > 2 && Pontos < 5){

                    DrawTexture(fundo2, 0, 0, WHITE);
                    if (gameOver) {
                        DesenhaJogo(&jogo, maca);
                        AtualizaRodada(&jogo);

                        if (ColisaoFood(&jogo)) {
                            PlaySound(somComer);
                            IniciaFood(&jogo);
                            AumentaSnake(&jogo);
                            Pontos++; //atualiza pontuação
                        }

                        //mostra pontuação:
                        sprintf(PontoNaTela, "Score: %d", Pontos);
                        DrawText(PontoNaTela, 10, 10, 30, WHITE);

                        ColisaoBordas(&jogo);
                        if (ColisaoSnake(&jogo)) {
                            PlaySound(somMorrer2);
                            gameOver = 0;
                        }

                    }else { //quando o jogador perde:
                        DrawText("FIM DE JOGO", 150, 200, 60, RED);
                        DrawText("Pressione Enter para voltar ao menu", 110, 400, 25, WHITE); //texto, x, y, tam fonte, cor
                        if (IsKeyPressed(KEY_ENTER)) {
                            atualizarRanking("ranking.txt", Nome, Pontos);                                      
                            estado = MENU;
                            Nome[0] = '\0'; // limpa o nome pra próxima partida!
                            tamanhoNome = 0;
                        }
                    }
                
                }else if(Pontos >= 5){

                    DrawTexture(fundo3, 0, 0, WHITE);
                    if (gameOver) {
                        DesenhaJogo(&jogo, maca);
                        AtualizaRodada(&jogo);

                        if (ColisaoFood(&jogo)) {
                            PlaySound(somComer);
                            IniciaFood(&jogo);
                            AumentaSnake(&jogo);
                            Pontos++; //atualiza pontuação
                        }

                        //mostra pontuação:
                        sprintf(PontoNaTela, "Score: %d", Pontos);
                        DrawText(PontoNaTela, 10, 10, 30, WHITE);

                        ColisaoBordas(&jogo);
                        if (ColisaoSnake(&jogo)) {
                            PlaySound(somMorrer3);
                            gameOver = 0;
                        }

                        }else{
                        DrawText("FIM DE JOGO", 150, 200, 60, RED);
                        DrawText("Pressione Enter para voltar ao menu", 110, 350, 25, WHITE); //texto, x, y, tam fonte, cor
                        if (IsKeyPressed(KEY_ENTER)) { 
                            atualizarRanking("ranking.txt", Nome, Pontos);                                     
                            estado = MENU;
                            Nome[0] = '\0'; // limpa o nome pra próxima partida!
                            tamanhoNome = 0;
                        }
                    } 
                }
                break;
                

            case RANKING:
                desenhaTelaRanking();
                if (IsKeyPressed(KEY_ESCAPE)){
                    estado = MENU;
                }
                break;
        }

        EndDrawing();
    }

    UnloadSound(somComer);//libera audios
    UnloadSound(somMorrer1);
    UnloadSound(somMorrer2);
    UnloadSound(somMorrer3);

    UnloadTexture(maca); // libera as texturas
    UnloadTexture(fundo1);
    UnloadTexture(fundo2);
    UnloadTexture(fundo3);

    FreeLista(&jogo.snake); 

    UnloadMusicStream(trilha);
    CloseAudioDevice();

    CloseWindow();
    return 0;
}
