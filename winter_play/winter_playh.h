//
// Created by shenjiawei on 2022/12/30.
//

#ifndef WINTER_PLAY_WINTER_PLAYH_H
#define WINTER_PLAY_WINTER_PLAYH_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include <stdbool.h>
#include <windows.h>

#undef main
enum window_size {
    Height = 730,
    Width = 1230
};
enum ReactionType {
    burning,
    booming,
    linking,
    crystal,
    increase,
    spreade
};
static SDL_Window *Window = NULL;
static SDL_Renderer *Renderer = NULL;
static SDL_Event Event;
//初始化窗口和渲染器，全局静态访问方式
static TTF_Font *LargeFont = NULL;
static TTF_Font *ChineseFont = NULL;
//static TTF_Font *SmallFont = NULL;
//static SDL_Color FontCgColor = {0, 0, 0, 0xFF};
static SDL_Color whiteTEX = {255, 255, 255, 255};
static SDL_Color blackTEX = {0, 0, 0, 255};
static SDL_Color yellowTEX = {255, 255, 0, 255};
static SDL_Color greenTEX = {0, 255, 0, 255};
static SDL_Color purpleTEX = {255, 0, 255, 255};
static SDL_Color redTEX = {255, 0, 0, 255};
//准备多种字体颜色
static Mix_Music *BGM[30] = {NULL};
static Mix_Chunk *chunks[10] = {NULL};
static SDL_Texture *BackIMG[30] = {NULL};
static SDL_Texture *Ball[8] = {NULL};
static SDL_Texture *bricks[701] = {NULL};
static SDL_Texture *Board[8] = {NULL};
static SDL_Texture *Setpage = NULL;
static SDL_Texture *SetButton = NULL;
static SDL_Texture *HelpButton = NULL;
static SDL_Texture *cTexture = NULL;
static SDL_Texture *TextALot[30] = {NULL};
static SDL_Texture *starting = NULL;
static SDL_Texture *BGbutton = NULL;
static SDL_Texture *Shield = NULL;
static SDL_Rect RectBG = {0, 0, Width, Height};
static SDL_Rect ScoreRect = {0, Height - 20, 0, 20};
static SDL_Rect ButtonRect = {890, 580, 180, 110};
static SDL_Rect ButtRect = {2, 2, 40, 40};
static SDL_Rect HelpRect = {2, 45, 40, 40};
static SDL_Rect CrossRect = {1190, 5, 38, 38};
static SDL_Rect PAUSERect = {365, 250, 500, 200};
static SDL_Rect EscRect = {410, 287, 123, 115};
static SDL_Rect ContinueRect = {570, 287, 123, 115};
static SDL_Rect RestartRect = {713, 287, 123, 115};
static SDL_Texture *Cross = NULL;
static SDL_Texture *GetTime = NULL;
static SDL_Texture *Pause_and_Restart = NULL;
static int copyq[15][20];
static int tmp1;
static clock_t game_start;
static int MyScore;
static int zhongli = 2;
typedef struct RC {
    int count;
    enum ReactionType type;
    SDL_Rect rct;
} R_C;
static R_C Reaction_counter[30];
//给大量的运动对象给予容器


enum TexColor {
    black,
    white,
    yellow,
    green,
    purple,
    red
};

enum InputAction {
    MoveDown = 0x0001,
    MoveUp = 0x0002,
    MoveLeft = 0x0004,
    MoveRight = 0x0008,
    Shoot = 0x0010,
    FireBoard = 0x0020,
    WaterBoard = 0x0040,
    IceBoard = 0x0080,
    RockBoard = 0x0100,
    GrassBoard = 0x0200,
    WindBoard = 0x0400,
    ThunderBoard = 0x0800,
};


static bool IsSystemPausing = false;
static bool IsGameRunning = false;
static bool IsNotGameStarting = false;
static bool IsSettingGoing = false;
static bool setting = false, helping = false;
static char score[20];
static int RestBalls = 10;
static int DisappearBalls = 0;
static bool Skill_of_Rock = false;
static bool Is_Sound_Skill_Of_Rock_Going = false;
typedef struct timebomb {
    clock_t start;
    int time;
    bool *flag;
} Ti;
Ti MyTime[10];
clock_t now;
clock_t PauseTime;
clock_t stt, edd;
static bool TimeFlag = false;
static bool flagtemp[15][20] = {};
static int timehello;

void INITALL();//即初始化所有我要的控件，包括窗口和画笔
void LOADING();

SDL_Texture *TexManager(const char *);//简化用的函数
void EventSolve(int);

int GetKey(const Uint8 *);

void render();//把已有的所有纹理丢到屏幕上面去
void QUIT();//退出而已，别大惊小怪，texture没释放应该不要紧
//void GetBoardSpeed();

void TRANS();

void present(int i);

void AudioBroadcasting(const char *, int);

void Ignore();

void Revive();

void PutSettingPage();

void PutHelpPage();

void PutSetPage();

bool IsClicked(SDL_Rect, SDL_Event);

void TimerPrint();

void setClock(bool *, double, int);

bool checkClock(int);

SDL_Texture *allTexmanager(const char *, bool, enum TexColor);

void RestartGame();

#endif//WINTER_PLAY_WINTER_PLAYH_H
