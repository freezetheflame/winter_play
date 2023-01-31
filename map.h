//
// Created by shenjiawei on 2023/1/4.
//

#ifndef WINTER_PLAY_MAP_H
#define WINTER_PLAY_MAP_H

#include "winter_playh.h"

int Second[15][25] =
        {{0, 0, 0, 200, 200, 200, 200, 200, 200, 0, 0, 300, 300, 300, 300, 0, 0, 200, 0,   0,   0,   0,   0, 0, 0},
         {0, 0, 0, 200, 0,   0,   0,   0,   0,   0, 0, 300, 0,   0,   300, 0, 0, 200, 0,   0,   0,   0,   0, 0, 0},
         {0, 0, 0, 200, 0,   0,   0,   0,   0,   0, 0, 300, 0,   9,   300, 0, 0, 200, 0,   0,   0,   0,   0, 0, 0},
         {0, 0, 0, 200, 0,   8,   0,   0,   0,   0, 0, 300, 300, 300, 300, 0, 0, 200, 0,   0,   0,   0,   0, 0, 0},
         {0, 0, 0, 200, 0,   0,   0,   0,   0,   0, 0, 300, 0,   0,   0,   0, 0, 200, 0,   0,   0,   0,   0, 0, 0},
         {0, 0, 0, 200, 0,   0,   0,   0,   0,   0, 0, 300, 0,   0,   0,   0, 0, 200, 0,   0,   0,   0,   0, 0, 0},
         {0, 0, 0, 200, 200, 200, 200, 200, 200, 0, 0, 300, 0,   0,   0,   0, 0, 200, 0,   0},
         {0, 0, 0, 0,   0,   0,   0,   0,   0,   0, 0, 300, 0,   0,   0,   0, 0, 200, 200, 200, 200, 200, 200},
         {0, 0, 0, 0,   0,   0,   0,   0,   0,   0, 0, 0,   0,   0,   0,   0, 0, 0,   0,   0},
         {0, 0, 0, 0,   0,   0,   0,   0,   0,   0, 0, 0,   0,   0,   0,   0, 0, 0,   0,   0},
         {0, 0, 0, 0,   0,   0,   0,   0,   0,   0, 0, 0,   0,   0,   0,   0, 0, 0,   0,   0},
         {0, 0, 0, 0,   0,   0,   0,   0,   0,   0, 0, 0,   0,   0,   0,   0, 0, 0,   0,   0},
         {0, 0, 0, 0,   0,   0,   0,   0,   0,   0, 0, 0,   0,   0,   0,   0, 0, 0,   0,   0},
         {0, 0, 0, 0,   0,   0,   0,   0,   0,   0, 0, 0,   0,   0,   0,   0, 0, 0,   0,   0},
         {0, 0, 0, 0,   0,   0,   0,   0,   0,   0, 0, 0,   0,   0,   0,   0, 0, 0,   0,   0}
        };
int First[15][25]
        = {
                {0, 100, 100, 100, 0, 0,   0,   0,   0, 0, 0, 0,   0,   0,   0, 0, 500, 0,   0,   0},
                {0, 100, 100, 100, 0, 0,   0,   0,   0, 0, 0, 0,   100, 100, 0, 0, 500, 700, 0,   0},
                {0, 100, 100, 100, 0, 0,   0,   0,   0, 0, 0, 0,   100, 0,   0, 0, 0,   700, 0,   0},
                {0, 100, 100, 100, 0, 0,   0,   0,   0, 0, 0, 700, 700, 0,   0, 0, 0,   700, 700, 0},
                {0, 0,   0,   0,   0, 0,   0,   0,   0, 0, 0, 100, 700, 0,   0, 0, 0,   0,   0,   0},
                {0, 0,   9,   0,   0, 0,   0,   0,   0, 0, 0, 0,   0,   0,   0, 0, 0,   0,   0,   0},
                {0, 600, 600, 600, 0, 300, 300, 300, 0, 0, 0, 0,   0,   0,   0, 0, 0,   0,   0,   0},
                {0, 600, 600, 600, 0, 300, 300, 300, 0, 0, 0, 0,   0,   0,   0, 0, 0,   0,   0,   0},
                {0, 600, 600, 600, 0, 300, 300, 300, 0, 0, 0, 0,   0,   0,   0, 0, 0,   0,   0,   0},
                {0, 600, 300, 600, 0, 300, 700, 300, 0, 0, 0, 0,   0,   0,   0, 0, 0,   0,   0,   0},
                {0, 0,   0,   0,   0, 0,   0,   0,   0, 0, 0, 0,   0,   0,   0, 0, 0,   0,   0,   0},
                {0, 0,   0,   0,   0, 0,   0,   0,   0, 0, 0, 0,   0,   0,   0, 0, 0,   0,   0,   0},
                {0, 0,   0,   0,   0, 0,   0,   0,   0, 0, 0, 0,   0,   0,   0, 0, 0,   0,   0,   0},
                {0, 0,   0,   0,   0, 0,   0,   0,   0, 0, 0, 0,   0,   0,   0, 0, 0,   0,   0,   0},
                {0, 0,   0,   0,   0, 0,   0,   0,   0, 0, 0, 0,   0,   0,   0, 0, 0,   0,   0,   0}

        };
int Third[15][25] =
        {{0, 600, 0,   0,   8,   600, 0,   0, 0,   0,   0,   500, 0,   0, 0, 700, 0,   0,   0,   700, 0, 0, 0},
         {0, 600, 600, 0,   0,   600, 0,   0, 0,   0,   0,   500, 0,   0, 0, 700, 0,   0,   0,   700, 0, 0, 0},
         {0, 600, 0,   600, 0,   600, 0,   0, 0,   0,   9,   500, 0,   0, 0, 700, 0,   0,   0,   700, 0, 0, 0},
         {0, 600, 0,   600, 0,   600, 0,   0, 0,   0,   0,   500, 0,   0, 0, 700, 0,   0,   0,   700, 0, 0, 0},
         {0, 600, 0,   0,   600, 600, 0,   0, 500, 0,   0,   500, 0,   0, 0, 700, 0,   0,   0,   700, 0, 0, 0},
         {0, 600, 0,   0,   0,   600, 0,   0, 0,   500, 500, 500, 0,   0, 0, 0,   700, 700, 700, 0,   0, 0, 0},
         {0, 0,   0,   0,   0,   0,   0,   0, 0,   0,   0,   0,   0,   0, 0, 0,   0,   0,   0,   0},
         {0, 0,   0,   0,   0,   0,   100, 0, 100, 100, 100, 0,   100, 0, 0, 0,   0,   0,   0,   0,   0, 0, 0, 0},
         {0, 0,   0,   0,   0,   0,   100, 0, 0,   0,   100, 0,   100, 0, 0, 0,   0,   0,   0,   0,   0, 0, 0, 0},
         {0, 0,   0,   0,   0,   0,   100, 0, 100, 100, 100, 0,   100, 0, 0, 0,   0,   0,   0,   0,   0, 0, 0, 0},
         {0, 0,   0,   0,   0,   0,   100, 0, 100, 0,   0,   0,   100, 0, 0, 0,   0,   0,   0,   0,   0, 0, 0, 0},
         {0, 0,   0,   0,   0,   0,   100, 0, 100, 100, 100, 0,   100, 0, 0, 0,   0,   0,   0,   0,   0, 0, 0, 0},
         {0, 0,   0,   0,   0,   0,   0,   0, 0,   0,   0,   0,   0,   0, 0, 0,   0,   0,   0,   0},
         {0, 0,   0,   0,   0,   0,   0,   0, 0,   0,   0,   0,   0,   0, 0, 0,   0,   0,   0,   0},
         {0, 0,   0,   0,   0,   0,   0,   0, 0,   0,   0,   0,   0,   0, 0, 0,   0,   0,   0,   0}
        };
SDL_Rect Map_Rect;

typedef struct Map {
    int arr[15][25];
    struct Map *next;
} Mapping;
Mapping *new;

Mapping *creat(int n) {
    Mapping *head, *node, *end;
    head = malloc(sizeof(Mapping));
    end = head;
    for (int i = 0; i < n; i++) {
        node = malloc(sizeof(Mapping));
        end->next = node;
        end = node;
    }
    end->next = NULL;//结束创建
    return head;
}

void MapInitial() {
    new = creat(10);
    Mapping *head = new;
    for (int row = 0; row < 15; row++)
        for (int column = 0; column < 25; column++) {
            new->arr[row][column] = First[row][column];
        }
    new = new->next;
    for (int row = 0; row < 15; row++)
        for (int column = 0; column < 25; column++) {
            new->arr[row][column] = Second[row][column];
        }
    new = new->next;
    for (int row = 0; row < 15; row++)
        for (int column = 0; column < 25; column++) {
            new->arr[row][column] = Third[row][column];
        }
    new = new->next;
    for (int row = 0; row < 15; row++)
        for (int column = 0; column < 25; column++) {
            new->arr[row][column] = First[row][column];
        }
    new = new->next;
    for (int row = 0; row < 15; row++)
        for (int column = 0; column < 25; column++) {
            new->arr[row][column] = First[row][column];
        }
    new = new->next;
    for (int row = 0; row < 15; row++)
        for (int column = 0; column < 25; column++) {
            new->arr[row][column] = First[row][column];
        }
    new = new->next;
    for (int row = 0; row < 15; row++)
        for (int column = 0; column < 25; column++) {
            new->arr[row][column] = First[row][column];
        }
    new = new->next;
    for (int row = 0; row < 15; row++)
        for (int column = 0; column < 25; column++) {
            new->arr[row][column] = First[row][column];
        }
    new = new->next;
    for (int row = 0; row < 15; row++)
        for (int column = 0; column < 25; column++) {
            new->arr[row][column] = First[row][column];
        }
    new = new->next;
    for (int row = 0; row < 15; row++)
        for (int column = 0; column < 25; column++) {
            new->arr[row][column] = First[row][column];
        }
    new = head;
}

Mapping *nownode = NULL;

void DrawBricks(int arr[15][25]) {
    Map_Rect.w = 46;
    Map_Rect.h = 46;
    for (int row = 0; row < 25; row++)
        for (int column = 0; column < 15; column++) {
            Map_Rect.x = row * 48;
            Map_Rect.y = column * 48;
            if (arr[column][row] != 0) {
                if(arr[column][row]==8||arr[column][row]==9)
                    SDL_RenderCopy(Renderer,gift,NULL,&Map_Rect);
                    else SDL_RenderCopy(Renderer, bricks[arr[column][row]], NULL, &Map_Rect);
            }

        }

}

bool IsCopyClear() {
    bool flag = true;
    for (int row = 0; row < 25; row++)
        for (int column = 0; column < 15; column++) {
            if (copyq[column][row] != 0)flag = false;
        }
    return flag;
}


void StructCopy(int arr[15][25]) {
    for (int row = 0; row < 15; row++)
        for (int column = 0; column < 25; column++)
            copyq[row][column] = arr[row][column];
}

void DoneALL() {
    Mix_PlayChannel(-1, chunks[6], 0);
    for (int i = 1; i <= 5000; i++) {
        SDL_PollEvent(&Event);
        SDL_Delay(1);
    }
    for (int row = 0; row < 15; row++)
        for (int column = 0; column < 25; column++) {
            if (copyq[row][column] != 0)MyScore += 2;
            copyq[row][column] /= 100;
        }
}

#endif//WINTER_PLAY_MAP_H
