#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"

/* Simulador de Colisões - Diego Camargo Dolzan*/

typedef struct
{
    Vector2 pos;
    Vector2 vel;
    float raio;
    Color cor;
}   Bola;




void FazColisao(Bola *a, Bola *b, float distX, float distY);
void CriaBolas(int n, int alt, int larg, Bola* bolas);



int main()
{

    int n_bolas, altura, largura;
    printf("\n Numero de bolas:");
    scanf("%d", &n_bolas);
    printf("\n Largura da Janela:");
    scanf("%d", &largura);
    printf("\n Altura da Janela: ");
    scanf("%d", &altura);

    InitWindow(largura, altura, "Simulador de Colisões");
    SetTargetFPS(60);
    //cria a janela com as informações lidas

    Bola* bolas;
    bolas=(Bola*)malloc (sizeof(Bola)*n_bolas);
    //aloca dinamicamente o vetor de bolas


    CriaBolas(n_bolas, altura, largura, bolas);
    //Cria as bolas, com posição, velocidade e cores aleatórias, e armazena no vetor

    while (!WindowShouldClose())
    {

        float dt = GetFrameTime();
        // intervalo de tempo


        for (int i = 0; i < n_bolas; i++)
        {

            // Move as bolinhas
            bolas[i].pos.x += bolas[i].vel.x * dt;
            bolas[i].pos.y += bolas[i].vel.y * dt;


            // Colisões com Paredes, mais fácil, nem precisa de função
            if (bolas[i].pos.x < bolas[i].raio)
                bolas[i].vel.x *= -1, bolas[i].pos.x = bolas[i].raio;
            if (bolas[i].pos.x > largura - bolas[i].raio)
                bolas[i].vel.x *= -1, bolas[i].pos.x = largura-bolas[i].raio;
            if (bolas[i].pos.y < bolas[i].raio)
                bolas[i].vel.y *= -1, bolas[i].pos.y = bolas[i].raio;
            if (bolas[i].pos.y > altura - bolas[i].raio)
                bolas[i].vel.y *= -1, bolas[i].pos.y = altura-bolas[i].raio;
            //Inverte a velocidade no contato com a parede, e redefine a posicao pra evitar que as bolas entrem nas paredes se forem "esmagadas" por outra bola
        }


        //verifica se teve alguma colisão entre as bolinhas, chama a função de colisão
        for (int i = 0; i < n_bolas; i++)
        {
            for (int j = i+1; j < n_bolas; j++)
            {
                float disX = bolas[j].pos.x - bolas[i].pos.x;
                float disY = bolas[j].pos.y - bolas[i].pos.y;
                float r = bolas[i].raio + bolas[j].raio;
                float distanciaReal=sqrt((disX*disX)+(disY*disY));

                if (distanciaReal < r)
                    FazColisao(&bolas[i], &bolas[j], disX, disY);
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);
        //começo real da representação

        for (int i = 0; i < n_bolas; i++)
        {
            DrawCircleV(bolas[i].pos, bolas[i].raio, bolas[i].cor);
        }
        //desenha as bolinhas na tela, usando os dados do ultimo dt



        EndDrawing();
    }

    CloseWindow();
    free (bolas);
    return 0;
}
void FazColisao(Bola *a, Bola *b, float distX, float distY)
{
    float eixo = (distX*distX)+(distY*distY);


    float proj_aX = (((a->vel.x)*distX)+((a->vel.y)*distY))/eixo *distX;
    float proj_aY = (((a->vel.x)*distX)+((a->vel.y)*distY))/eixo *distY;
    float proj_bX = (((b->vel.x)*distX)+((b->vel.y)*distY))/eixo *distX;
    float proj_bY = (((b->vel.x)*distX)+((b->vel.y)*distY))/eixo *distY;
    //projeta as velocidades em X e Y

    a->vel.x=a->vel.x-proj_aX+proj_bX;
    a->vel.y=a->vel.y-proj_aY+proj_bY;
    b->vel.x=b->vel.x-proj_bX+proj_aX;
    b->vel.y=b->vel.y-proj_bY+proj_aY;
    //atualiza as velocidades usando as projecoes

    a->pos.x -=(distX / sqrt(eixo)) * 0.5;
    a->pos.y -=(distY / sqrt(eixo)) * 0.5;
    b->pos.x +=(distX / sqrt(eixo)) * 0.5;
    b->pos.y +=(distY / sqrt(eixo)) * 0.5;
    //corrige a posição pra evitar que as bolinhas entrem uma na outra




}
void CriaBolas(int n, int alt, int larg, Bola* bolas)
{
    for (int i = 0; i < n; i++)
    {
        bolas[i].raio = 40;
        bolas[i].pos.x = GetRandomValue(50, larg-50);
        bolas[i].pos.y = GetRandomValue(50, alt-50);
        bolas[i].vel.x = GetRandomValue(-300, 300);
        bolas[i].vel.y = GetRandomValue(-300, 300) ;
        bolas[i].cor.a = 255;
        bolas[i].cor.r = GetRandomValue(50, 250);
        bolas[i].cor.g = GetRandomValue(50, 250);
        bolas[i].cor.b = GetRandomValue(50, 250);

        for (int j=i-1; j>=0; j--)
        {
            float disX = bolas[j].pos.x - bolas[i].pos.x;
            float disY = bolas[j].pos.y - bolas[i].pos.y;
            float r = bolas[i].raio + bolas[j].raio;
            float distanciaReal=sqrt((disX*disX)+(disY*disY));

            if (distanciaReal < r)
            {
                i--;
                break;
            }

        }

    }
}

