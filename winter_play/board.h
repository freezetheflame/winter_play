//
// Created by shenjiawei on 2023/1/2.
//

#ifndef WINTER_PLAY_BOARD_H
#define WINTER_PLAY_BOARD_H

#include"winter_playh.h"


enum board_size {
    Length = Width / 6,
    thickness = 10
};
static int xpos1 = 600, xpos2, ypos1 = 700, ypos2;

void update();

static SDL_Rect BoardSet = {177, 0, Length, thickness};
enum elements {
    fire = 1,
    water,
    ice,
    rock,
    grass,
    wind,
    thunder
};
typedef struct BoardParameter {
    int length;
    enum elements ele;
    SDL_Rect BoardDesRect;
    int colour;
    int number;
    int direct;//三个速度方向：静止，左，右，依据这个加速
} BP;
BP player1, player2;

void SetPlayer() {
    player1.length = player2.length = Length;
    player2.number = player1.number = 1;
    player1.ele = player2.ele = fire;
    player1.BoardDesRect.w = 165;
    player1.BoardDesRect.h = 20;
    player1.BoardDesRect.x = xpos1;
    player1.BoardDesRect.y = ypos1;
    player2.BoardDesRect.w = 165;
    player2.BoardDesRect.h = 20;
    player2.BoardDesRect.x = xpos2;
    player2.BoardDesRect.y = ypos2;
}

int P1Board = 1, P2Board = 1;
//不务正业的Board，Ball实在太大了和这个Board差异太大了
int ReactionNow = 0;

void enter(int clock_num, SDL_Rect RECT, enum ReactionType rctype) {
    Reaction_counter[clock_num].count = 60;//持续60帧即可
    Reaction_counter[clock_num].type = rctype;
    Reaction_counter[clock_num].rct = RECT;
}

void ReactionPV(enum ReactionType i, SDL_Rect Rect) {
    ReactionNow++;
    enter(ReactionNow, Rect, i);
}

void ReactionClockCheck() {
    for (int i = 1; i <= ReactionNow; i++)
        if (Reaction_counter[i].count > 0) {
            Reaction_counter[i].count--;
            SDL_Texture *boom=NULL;
            switch (Reaction_counter[i].type) {
                case booming:
                    boom = allTexmanager("BOOM!",false,yellow);
                    Reaction_counter[i].rct.w=80;
                    Reaction_counter[i].rct.h=20;
                    SDL_RenderCopy(Renderer,boom,NULL,&Reaction_counter[i].rct);
                    SDL_DestroyTexture(boom);
                case burning:
                    boom = allTexmanager("BURN!",false,red);
                    Reaction_counter[i].rct.w=80;
                    Reaction_counter[i].rct.h=20;
                    SDL_RenderCopy(Renderer,boom,NULL,&Reaction_counter[i].rct);
                    SDL_DestroyTexture(boom);
                    break;
                case linking:
                    boom = allTexmanager("LINK!",false,purple);
                    Reaction_counter[i].rct.w=80;
                    Reaction_counter[i].rct.h=20;
                    SDL_RenderCopy(Renderer,boom,NULL,&Reaction_counter[i].rct);
                    SDL_DestroyTexture(boom);
                    break;
                case crystal:
                    boom = allTexmanager("天动万象！",true,yellow);
                    Reaction_counter[i].rct.w=80;
                    Reaction_counter[i].rct.h=16;
                    SDL_RenderCopy(Renderer,boom,NULL,&Reaction_counter[i].rct);
                    SDL_DestroyTexture(boom);
                    break;
                case increase:
                    boom = allTexmanager("DOUB!",false,red);
                    Reaction_counter[i].rct.w=80;
                    Reaction_counter[i].rct.h=20;
                    SDL_RenderCopy(Renderer,boom,NULL,&Reaction_counter[i].rct);
                    SDL_DestroyTexture(boom);
                    break;
                case spreade:
                    boom = allTexmanager("诶嘿！",true,green);
                    Reaction_counter[i].rct.w=80;
                    Reaction_counter[i].rct.h=20;
                    SDL_RenderCopy(Renderer,boom,NULL,&Reaction_counter[i].rct);
                    SDL_DestroyTexture(boom);
                    break;
            }
        }
        else if(Reaction_counter[i].count==0)
        {
            for(int j=i;j<=ReactionNow;j++)
                Reaction_counter[j]=Reaction_counter[j+1];//整体前移
            ReactionNow--;
        }
}

#endif//WINTER_PLAY_BOARD_H
