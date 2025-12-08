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

/*void RegistrarCurvaSeDirecaoMudou(Jogo *j) {
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
}*/

void RegistrarCurvaSeDirecaoMudou(Jogo *j) {
    CelulaSnake *cab = j->snake.Cabeca;
    int dirAtual = cab->body.direcao;
    int nova = dirAtual;

    if (IsKeyDown(KEY_UP)    && dirAtual != BAIXO)    nova = CIMA;
    if (IsKeyDown(KEY_DOWN)  && dirAtual != CIMA)     nova = BAIXO;
    if (IsKeyDown(KEY_LEFT)  && dirAtual != DIREITA)  nova = ESQUERDA;
    if (IsKeyDown(KEY_RIGHT) && dirAtual != ESQUERDA) nova = DIREITA;

    if (nova != dirAtual) {
        // Snap (alinhar) a posição da cabeça ao grid antes de registrar a curva
        int curveX = (((int)cab->body.pos.x) / STD_SIZE_X) * STD_SIZE_X;
        int curveY = (((int)cab->body.pos.y) / STD_SIZE_Y) * STD_SIZE_Y;
        AdicionaCurva(curveX, curveY, nova);
        cab->body.direcao = nova;
    }
}

void AplicarCurvasNosSegmentos(Jogo *j) {
    if (!curvas) return;

    CelulaSnake *corpo = j->snake.Cabeca->Prox;
    while (corpo) {
        Curva *c = curvas;
        while (c) {
            // arredondamos / alinhamos a posição atual do segmento ao grid
            int segX = (((int)corpo->body.pos.x) / STD_SIZE_X) * STD_SIZE_X;
            int segY = (((int)corpo->body.pos.y) / STD_SIZE_Y) * STD_SIZE_Y;

            if (segX == c->x && segY == c->y) {
                corpo->body.direcao = c->direcao;
                if (corpo->Prox == NULL) { // rabo
                    RemoveCurva(c);
                }
                break; // saiu do loop de curvas para este segmento
            }
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


void DesenhaCobra(Jogo *j) {
    CelulaSnake *celula = j->snake.Cabeca;

    while (celula) {
        Rectangle hitbox = celula->body.pos;   // usado para colisão
        Rectangle src;
        
        // novo rectangle APENAS para desenhar
        Rectangle dstDraw = {
            hitbox.x + hitbox.width / 2.0f,    // compensação
            hitbox.y + hitbox.height / 2.0f,   // compensação
            hitbox.width,
            hitbox.height
        };

        Vector2 origin = { hitbox.width / 2.0f, hitbox.height / 2.0f };
        float ang = DirecaoParaAngulo(celula->body.direcao);

        if (celula == j->snake.Cabeca) {
            src = (Rectangle){0, 0, j->tex.Cabeca.width, j->tex.Cabeca.height};
            DrawTexturePro(j->tex.Cabeca, src, dstDraw, origin, ang, WHITE);
        }
        else if (celula->Prox == NULL) {
            src = (Rectangle){0, 0, j->tex.Rabo.width, j->tex.Rabo.height};
            DrawTexturePro(j->tex.Rabo, src, dstDraw, origin, ang, WHITE);
        }
        else {
            src = (Rectangle){0, 0, j->tex.Corpo.width, j->tex.Corpo.height};
            DrawTexturePro(j->tex.Corpo, src, dstDraw, origin, ang, WHITE);
        }

        celula = celula->Prox;
    }
}
