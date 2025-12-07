#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "listacobra.h"
#include "giracobra.h"
#include <math.h>

#include "ranking.h"
#include "menu.h"

int Pontos;
char Nome[50];


int main(){

    srand(time(NULL)); 
    Jogo jogo;
    jogo.LARGURA = 660;
    jogo.ALTURA = 660;
    jogo.escala = jogo.LARGURA / 660.0f;
    int gameOver = 1;
    Pontos=0;
    char PontoNaTela[20];
    int tamanhoNome=0;
    jogo.barreiras[0].inicia = 0;
    jogo.barreiras[1].inicia = 0;

    //Cria a janela do jogo;
    InitWindow(jogo.LARGURA, jogo.ALTURA, "Snake Game");

    InitAudioDevice();   // inicializa o sistema de som
    Music musmenu = LoadMusicStream("Assets/musmenu.mp3");
    Music trilha1 = LoadMusicStream("Assets/trilha1.mp3");
    Music trilha2 = LoadMusicStream("Assets/trilha2.mp3");
    Music trilha3 = LoadMusicStream("Assets/trilha3.mp3");

    PlayMusicStream(musmenu);
    PlayMusicStream(trilha1);
    PlayMusicStream(trilha2);
    PlayMusicStream(trilha3);

    SetMusicVolume(musmenu, 0.0f);
    SetMusicVolume(trilha1, 0.0f);
    SetMusicVolume(trilha2, 0.0f);
    SetMusicVolume(trilha3, 0.0f);


    SetExitKey(KEY_NULL);//pra não dar o bug do esc sempre fechar o jogo.
    SetTargetFPS(50);
    srand(time(NULL));

    Sound somComer = LoadSound("Assets/somComer.mp3");
    Sound somMorrer1 = LoadSound("Assets/somMorrer1.mp3");
    Sound somMorrer2 = LoadSound("Assets/somMorrer2.mp3");
    Sound somMorrer3 = LoadSound("Assets/somMorrer3.mp3");

    Texture2D fundo1 = LoadTexture("Assets/GramaFundo.jpeg");
    Texture2D fundo2 = LoadTexture("Assets/fundoespaco.jpg");
    Texture2D fundo3 = LoadTexture("Assets/fundomar.jpeg");
    CarregaTexturas(&jogo);

    Estado estado= INTRO;
    while (!WindowShouldClose()) {

        UpdateMusicStream(musmenu);  
        UpdateMusicStream(trilha1);
        UpdateMusicStream(trilha2);
        UpdateMusicStream(trilha3);
        BeginDrawing();
        ClearBackground(RAYWHITE);

    switch (estado) { //determina o que fazer em cada estado do jogo:
        case INTRO:
        SetMusicVolume(musmenu, 0.5f);
        SetMusicVolume(trilha1, 0.0f);
        SetMusicVolume(trilha2, 0.0f);
        SetMusicVolume(trilha3, 0.0f);
        desenhaIntro(&jogo);
        if (IsKeyPressed(KEY_ENTER)) estado = MENU; //se apertar 1 abre o ranking,
        if (IsKeyPressed(KEY_ESCAPE)){
            CloseWindow();//fecha a tela com esc;
        } 
    break;
        case MENU:
            SetMusicVolume(musmenu, 0.5f);
            SetMusicVolume(trilha1, 0.0f);
            SetMusicVolume(trilha2, 0.0f);
            SetMusicVolume(trilha3, 0.0f);

            desenhaMenuPrincipal(&jogo);
                if (IsKeyPressed(KEY_ONE)) estado = RANKING; //se apertar 1 abre o ranking,
                if (IsKeyPressed(KEY_TWO)) estado = TELAS; //2 -> JOGAR! Pede o NOME antes! Por isso tem o caso NOME.
                if (IsKeyPressed(KEY_ESCAPE)){
                CloseWindow();//fecha a tela com esc;
            } 
        break;

        case TELAS:
            SetMusicVolume(musmenu, 0.5f);
            SetMusicVolume(trilha1, 0.0f);
            SetMusicVolume(trilha2, 0.0f);
            SetMusicVolume(trilha3, 0.0f);

            desenhaTelaTelas(&jogo);
            if(IsKeyPressed(KEY_ONE)){
                SetWindowSize(660, 660);
                jogo.LARGURA = 660;
                jogo.ALTURA = 660;
                jogo.escala = jogo.LARGURA / 660.0f;
                estado = NOME;
            }//comandos pra tela pequena

            if(IsKeyPressed(KEY_TWO)){
                SetWindowSize(800, 800);
                jogo.LARGURA = 800;
                jogo.ALTURA = 800;
                jogo.escala = jogo.LARGURA / 660.0f;
                estado = NOME;
            }
            //comando pro outro
            if(IsKeyPressed(KEY_ENTER)) estado = NOME;
            if(IsKeyPressed(KEY_ESCAPE)) estado = MENU;
        break;

        case NOME:
            SetMusicVolume(musmenu, 0.5f);
            SetMusicVolume(trilha1, 0.0f);
            SetMusicVolume(trilha2, 0.0f);
            SetMusicVolume(trilha3, 0.0f);

            desenhaTelaNome(&jogo, Nome, &tamanhoNome); //recebe o nome.
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


        case JOGO: 
             //JOGO 1
            if(Pontos <= 1){
                SetMusicVolume(musmenu, 0.0f);
                SetMusicVolume(trilha1, 0.5f);
                SetMusicVolume(trilha2, 0.0f);
                SetMusicVolume(trilha3, 0.0f);

                DesenhaFundo(&jogo, &fundo1);
                IniciaBarreiras1(&jogo);
                if (gameOver) {
                    DesenhaJogo(&jogo);
                    DesenhaBarreiras1(&jogo);
                    AtualizaRodada(&jogo);

                    if (ColisaoFood(&jogo)) {
                        PlaySound(somComer);
                        IniciaFood(&jogo);
                        AumentaSnake(&jogo);
                        Pontos++; //atualiza pontuação
                    }

                    //mostra pontuação:
                    sprintf(PontoNaTela, "Score: %d", Pontos);
                    DrawText(PontoNaTela, (10*jogo.escala), (10*jogo.escala), (30*jogo.escala), WHITE);

                    ColisaoBordas(&jogo);
                    if (ColisaoSnake(&jogo)  || ColisaoBarreiras1(&jogo)) {
                        PlaySound(somMorrer1);
                        gameOver = 0;
                    }

                }else{
                    DrawText("FIM DE JOGO", (150*jogo.escala), (200*jogo.escala), (60*jogo.escala), RED);
                    DrawText("Pressione Enter para voltar ao menu", (110*jogo.escala), (350*jogo.escala), (25*jogo.escala), WHITE); 
                    if (IsKeyPressed(KEY_ENTER)) { 
                        atualizarRanking("ranking.txt", Nome, Pontos);                                     
                        estado = MENU;
                        Nome[0] = '\0'; // limpa o nome pra próxima partida!
                        tamanhoNome = 0;
                    }
                }
                //JOGO 2
                else if(Pontos > 1 && Pontos < 5){
                SetMusicVolume(musmenu, 0.0f);
                SetMusicVolume(trilha1, 0.0f);
                SetMusicVolume(trilha2, 0.5f);
                SetMusicVolume(trilha3, 0.0f);

                DesenhaFundo(&jogo, &fundo2);
                if (gameOver) {
                DesenhaJogo2(&jogo);
                DesenhaBarreiras2(&jogo);
                AtualizaRodada(&jogo);

                if (ColisaoFood(&jogo)) {
                PlaySound(somComer);
                IniciaFood(&jogo);
                IniciaBarreiras2(&jogo);
                AumentaSnake(&jogo);
                Pontos++; //atualiza pontuação
                }

                //mostra pontuação:
                sprintf(PontoNaTela, "Score: %d", Pontos);
                DrawText(PontoNaTela, (10*jogo.escala), (10*jogo.escala), (30*jogo.escala), WHITE);

                ColisaoBordas(&jogo);
                if (ColisaoSnake(&jogo)) {
                PlaySound(somMorrer2);
                gameOver = 0;
                }

                }else { //quando o jogador perde:
                    DrawText("FIM DE JOGO", (150*jogo.escala), (200*jogo.escala), (60*jogo.escala), RED);
                    DrawText("Pressione Enter para voltar ao menu", (110*jogo.escala), (350*jogo.escala), (25*jogo.escala), WHITE); 
                    if (IsKeyPressed(KEY_ENTER)) {
                        atualizarRanking("ranking.txt", Nome, Pontos);                                      
                        estado = MENU;
                        Nome[0] = '\0'; // limpa o nome pra próxima partida!
                        tamanhoNome = 0;
                    }
                }
            }
            //JOGO 3
            else if(Pontos >= 5){
                SetMusicVolume(musmenu, 0.0f);
                SetMusicVolume(trilha1, 0.0f);
                SetMusicVolume(trilha2, 0.0f);
                SetMusicVolume(trilha3, 0.5f);
                
                DesenhaFundo(&jogo, &fundo3);
                if (gameOver) {
                    DesenhaJogo3(&jogo);
                    DesenhaBarreiras3(&jogo);
                    Barreiras3(&jogo);
                    AtualizaRodada(&jogo);

                    if (ColisaoFood(&jogo)) {
                        PlaySound(somComer);
                        IniciaFood(&jogo);
                        AumentaSnake(&jogo);
                        Pontos++; //atualiza pontuação
                    }

                    //mostra pontuação:
                    sprintf(PontoNaTela, "Score: %d", Pontos);
                    DrawText(PontoNaTela, (10*jogo.escala), (10*jogo.escala), (30*jogo.escala), WHITE);

                    ColisaoBordas(&jogo);
                    if (ColisaoSnake(&jogo) || ColisaoBarreiras3(&jogo)) {
                        PlaySound(somMorrer3);
                        gameOver = 0;
                    }

                }else{
                    DrawText("FIM DE JOGO", (150*jogo.escala), (200*jogo.escala), (60*jogo.escala), RED);
                    DrawText("Pressione Enter para voltar ao menu", (110*jogo.escala), (350*jogo.escala), (25*jogo.escala), WHITE); //texto, x, y, tam fonte, cor
                    if (IsKeyPressed(KEY_ENTER)) { 
                        atualizarRanking("ranking.txt", Nome, Pontos);                                     
                        estado = MENU;
                        Nome[0] = '\0'; // limpa o nome pra próxima partida!
                        tamanhoNome = 0;
                        jogo.barreiras[0].inicia = 0;
                        jogo.barreiras[1].inicia = 0;
                    }
                } 
            }
        break;


        case RANKING:
            SetMusicVolume(musmenu, 0.5f);
            SetMusicVolume(trilha1, 0.0f);
            SetMusicVolume(trilha2, 0.0f);
            SetMusicVolume(trilha3, 0.0f);

            desenhaTelaRanking(&jogo);
                if (IsKeyPressed(KEY_ENTER)){
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

    UnloadTexture(fundo1);
    UnloadTexture(fundo2);
    UnloadTexture(fundo3);

    FreeLista(&jogo.snake); 
    LiberaTexturas(&jogo);

    UnloadMusicStream(musmenu);
    UnloadMusicStream(trilha1);
    UnloadMusicStream(trilha2);
    UnloadMusicStream(trilha3);
    CloseAudioDevice();

    CloseWindow();
    return 0;
}
