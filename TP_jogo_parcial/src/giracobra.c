#include "giracobra.h"
#include <stdlib.h>
#include <math.h> // fmod - mexer com ângulos!
#include<stdio.h>

/*OBS: CONCEITO NOVO:
static eu uso quando quero que aquela variável/função só apareça dentro daquele pedaço de código,
uso pra nenhuma outra parte do código "puxar" esses conteúdos, e, quando uso, não preciso declarar
essas coisas no .h!*/

typedef struct Curva {
    int x, y;       // "quadrado" da curva;
    int direcao;    // direção pra virar;
    struct Curva *prox; //aponta pra próxima curva;
} Curva;

static Curva *curvas = NULL; //"curvas ativas no momento";


//recebe o comandinho do teclado e transforma num ângulo pra girar a imagem em cada caso;
float DirecaoParaAngulo(int d){
    switch(d){
        case CIMA:     return 0.0f;     // cabeça apontada para cima
        case DIREITA:  return 90.0f;    // direita
        case BAIXO:    return 180.0f;   // baixo
        case ESQUERDA: return -90.0f;   // esquerda
        default:       return 0.0f;
        //por que desse default? -  sem default, se cair num valor sem ser cima,  baixo..., 
        //a função não retornaria nada ou daria erro. Nesse caso, ela só não roda e pronto;

    }
}

/*funciona como um registro de curvas:
“Neste quadrado, a cobra virou p/ direção tal."
Esse registro fica guardado numa lista: a "curvas" criada lá em cima;
Quando cadda parte da cobra passa por esse quadrado, ela "consulta" a lista e muda de direção de acordo.
O rabo "limpa" a curva quando passa, para não acumular lixo entre uma curva e a próxima: remove o nó da curva que já foi da lista!!!
 */

void AdicionaCurva(int x, int y, int direcao) {
    Curva *c = (Curva*)malloc(sizeof(Curva));
    c->x = x; c->y = y; c->direcao = direcao;
    c->prox = curvas;
    curvas = c;
}

static void RemoveCurva(Curva *tirar) {
    //se não tem nadda pra tirar, saio!
    if (!curvas || !tirar) return;
    //se o primeiro da lista for o que é pra tirar,
    //curva passa a apontar pro próximo nó e eu tiro ele;
    if (curvas == tirar) {
        curvas = curvas->prox;
        free(tirar);
        return;
    }
    //caso contrário, faço um p temporário pra percorrer a lista,
    //quando acho, faço o nó que aponta pra ele apontar pro próximo depois dele e tiro ele;
    Curva *p = curvas;
    while (p->prox && p->prox != tirar) p = p->prox;
    if (p->prox == tirar) {
        p->prox = tirar->prox;
        free(tirar);
    }
}


void RegistrarCurvaSeDirecaoMudou(Jogo *j) {
    CelulaSnake *cab = j->snake.Cabeca;
    int dirAtual = cab->body.direcao;
    int nova = dirAtual;

    if (IsKeyDown(KEY_UP)    && dirAtual != BAIXO)    nova = CIMA;
    if (IsKeyDown(KEY_DOWN)  && dirAtual != CIMA)     nova = BAIXO;
    if (IsKeyDown(KEY_LEFT)  && dirAtual != DIREITA)  nova = ESQUERDA;
    if (IsKeyDown(KEY_RIGHT) && dirAtual != ESQUERDA) nova = DIREITA;

    if (nova != dirAtual) {
        AdicionaCurva((int)cab->body.pos.x, (int)cab->body.pos.y, nova);
        cab->body.direcao = nova;
    }
}

void AplicarCurvasNosSegmentos(Jogo *j) {
    if (!curvas) return;

    CelulaSnake *corpo = j->snake.Cabeca->Prox;
    while (corpo) {
        Curva *c = curvas;
        while (c) {
            //confiro se chegou no "quadrado de virada" e viro o corpo todo:
        if ((int)corpo->body.pos.x == c->x && (int)corpo->body.pos.y == c->y) {
            corpo->body.direcao = c->direcao;
            if (corpo->Prox == NULL) { // rabo
                RemoveCurva(c);
            }
            break; // sai do loop de curvas para este segmento
        }
            //passo pra próxima curva da lista!
            c = c->prox;
        }
        corpo = corpo->Prox;
    }
}

void LimparCurvas(void) {
    Curva *c = curvas;
    while (c) {
        Curva *n = c->prox;
        free(c);
        c = n;
    }
    curvas = NULL;
}

void DesenhaCobra(Jogo *j, Texture2D texCabeca, Texture2D texCorpo, Texture2D texRabo) {
    CelulaSnake *celula = j->snake.Cabeca;
    while (celula) {
        Rectangle src, dst = celula->body.pos;
        //src:dimensões da textura
        //dst:onde e em que tamanho ela vai aparecer na tela do jogo
        Vector2 origin = { dst.width/2.0f, dst.height/2.0f };

        float ang = DirecaoParaAngulo(celula->body.direcao);

        if (celula == j->snake.Cabeca) {
            src = (Rectangle){0, 0, (float)texCabeca.width, (float)texCabeca.height};
            DrawTexturePro(texCabeca, src, dst, origin, ang, WHITE);
        }
        else if (celula->Prox == NULL) {
            src = (Rectangle){0, 0, (float)texRabo.width, (float)texRabo.height};
            DrawTexturePro(texRabo, src, dst, origin, ang, WHITE);
        }
        else {
            src = (Rectangle){0, 0, (float)texCorpo.width, (float)texCorpo.height};
            DrawTexturePro(texCorpo, src, dst, origin, ang, WHITE);
        }
        celula = celula->Prox;
    }
}
/* DEBUG:
void DesenhaCobra(Jogo *j, Texture2D texCabeca, Texture2D texCorpo, Texture2D texRabo) {
    CelulaSnake *celula = j->snake.Cabeca;
    int i = 0;

    while (celula) {
        Rectangle dst = celula->body.pos;

        // DEBUG: desenhar quadrados coloridos em vez de textura
        Color cor;
        if (celula == j->snake.Cabeca) cor = RED;        // cabeça
        else if (celula->Prox == NULL) cor = BLUE;       // rabo
        else cor = GREEN;                                // corpo

        DrawRectangle((int)dst.x, (int)dst.y,
                      (int)dst.width, (int)dst.height,
                      cor);

        // DEBUG extra: imprimir no console
        printf("Segmento %d: x=%.0f y=%.0f dir=%d\n",
               i, dst.x, dst.y, celula->body.direcao);

        celula = celula->Prox;
        i++;
    }
}*/
