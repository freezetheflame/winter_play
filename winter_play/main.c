#include"winter_playh.h"
#include"board.h"
#include "Ball.h"
#include "map.h"
#include "tools.h"

int main() {
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;
    const Uint8 *KeyValue = SDL_GetKeyboardState(NULL);
    INITALL();
    LOADING();
    SDL_Event event;
    if (Mix_PausedMusic())
        Mix_ResumeMusic();
    else
        Mix_PlayMusic(BGM[1], -1);
    while (IsNotGameStarting) {
        SDL_RenderCopy(Renderer, starting, NULL, &RectBG);
        SDL_RenderPresent(Renderer);
        SDL_PollEvent(&event);
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            IsNotGameStarting = false;
            Mix_HaltMusic();
            SDL_Delay(50);
            Mix_PlayChannel(-1, chunks[1], 0);
        } else if (event.type == SDL_QUIT) {
            QUIT();
            return 0;
        }

    }
    Ignore();
    //TRANS();
    Revive();
    while (IsSettingGoing) {
        if (!setting && !helping)PutSettingPage();
        SDL_PollEvent(&Event);
        if (helping)PutHelpPage();
        if (setting)PutSetPage();
        if (Event.type == SDL_MOUSEBUTTONDOWN && IsClicked(ButtonRect, Event) && !setting && !helping)
            IsSettingGoing = false;
        else if (Event.type == SDL_QUIT) {
            QUIT();
            return 0;
        } else if (Event.type == SDL_KEYDOWN || Event.type == SDL_MOUSEBUTTONDOWN) {
            if ((Event.key.keysym.sym == SDLK_h && !setting) || (!helping && IsClicked(HelpRect, Event)) || (helping &&
                                                                                                             IsClicked(
                                                                                                                     CrossRect,
                                                                                                                     Event))) {
                helping = (helping == true ? false : true);
            } else if ((Event.key.keysym.sym == SDLK_s && !helping) || (!setting && IsClicked(ButtRect, Event)) ||
                       (setting &&
                        IsClicked(CrossRect, Event))) {
                setting = (setting == true ? false : true);
            }
        }
    }
    Mix_PlayMusic(BGM[2], -1);
    Mix_ResumeMusic();
    timehello = 1;
    while (IsGameRunning) {
        if (timehello)game_start = clock();
        timehello = 0;
        frameStart = SDL_GetTicks();
        SDL_PumpEvents();
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            QUIT();
            break;
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_p) {
                IsSystemPausing = IsSystemPausing == true ? false : true;
                if (Mix_PausedMusic())Mix_ResumeMusic();
                else Mix_PauseMusic();
            }
        }
        if (IsSystemPausing) {
            if (event.type == SDL_MOUSEBUTTONDOWN && IsClicked(ContinueRect, event)) {
                IsSystemPausing = false;
                Mix_ResumeMusic();
            } else if (event.type == SDL_MOUSEBUTTONDOWN && IsClicked(EscRect, event)) {
                IsGameRunning = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && IsClicked(RestartRect, event)) {

                Mix_PlayChannel(-1, chunks[5], 0);
                for (int j = 1; j <= 2000; j++) {
                    SDL_PollEvent(&Event);
                    SDL_Delay(1);
                }
                RestartGame();
                IsSystemPausing = false;
            }
        }
        EventSolve(GetKey(KeyValue));
        update();
        render();
        frameTime = (int) SDL_GetTicks() - (int) frameStart;

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    QUIT();
    return 0;
}

void INITALL() {
    //SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    Mix_VolumeMusic(40);
    Mix_Volume(-1, 40);
    TTF_Init();
    Mix_Init(MIX_INIT_MP3);
    Window = SDL_CreateWindow("Dongjinyuxi", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height,
                              SDL_WINDOW_SHOWN);
    if (Window) printf("window sucessfully created!\n");
    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
    if (Renderer) printf("Renderer is sucessfully created!\n");
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048);
    //srand((unsigned) time(NULL));
    Initial_The_Map();
    StructCopy(hello);
}

void LOADING()//用途：把全部素材加载到渲染器中
{
    //char FILEname[40];//可以规律性地录入多组图片，现在先原始一点好了
    LargeFont = TTF_OpenFont("media/font/Blazma-Regular.ttf", 40);
    ChineseFont = TTF_OpenFont("media/font/ChineseFont.ttf", 40);
    BackIMG[0] = TexManager("media/image/cover.png");
    BackIMG[1] = TexManager("media/image/play(2).png");
    AudioBroadcasting("media/audio/background.mp3", 1);
    AudioBroadcasting("media/audio/fighting.mp3", 2);
    BGbutton = TexManager("media/image/BGbutton.png");
    chunks[1] = Mix_LoadWAV("media/audio/openingdoor.mp3");
    chunks[2] = Mix_LoadWAV("media/audio/skill1.mp3");
    chunks[3] = Mix_LoadWAV("media/audio/skill2.mp3");
    chunks[4] = Mix_LoadWAV("media/audio/skill3.mp3");
    chunks[5]= Mix_LoadWAV("media/audio/Restart.mp3");
    Shield = TexManager("media/image/sheild.png");
    for (int i = 1; i <= 7; i++) {
        char *s = malloc(30 * sizeof(char));
        sprintf(s, "media/image/brick%d.jpg", i);
        bricks[i] = TexManager(s);
        sprintf(s, "media/image/brick%d.jpg", i * 10);
        bricks[i * 10] = TexManager(s);
        sprintf(s, "media/image/brick%d.jpg", i * 100);
        bricks[i * 100] = TexManager(s);
        free(s);
    }
    for (int i = 1; i <= 7; i++) {
        char *s = malloc(30 * sizeof(char));
        sprintf(s, "media/image/board%d.png", i);
        Board[i] = TexManager(s);
    }
    for (int i = 1; i <= 7; i++) {
        char *s = malloc(30 * sizeof(char));
        sprintf(s, "media/image/ball%d.png", i);
        Ball[i] = TexManager(s);
    }
    starting = TexManager("media/image/startbackground.jpg");
    Cross = TexManager("media/image/cross.jpg");
    Setpage = TexManager("media/image/cover.png");
    SetButton = TexManager("media/image/setbutton.png");
    HelpButton = TexManager("media/image/help.png");
    BackIMG[4] = TexManager("media/image/setbbb.jpg");
    BackIMG[3] = TexManager("media/image/setbackground.jpg");
    TextALot[0] = allTexmanager("Adapted and simplified from Genshin Impact,only for study", false, white);
    TextALot[1] = allTexmanager("WASD对应上下左右移动,p键暂停", true, white);
    TextALot[2] = allTexmanager("游戏限时3min，最后将给出你的总得分", true, white);
    Pause_and_Restart = TexManager("media/image/PauseandRestart.png");
    IsGameRunning = true;
    IsNotGameStarting = true;
    IsSettingGoing = true;
    SetPlayer();
    CreateBall();
}

void QUIT() {
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    SDL_Quit();
}

//当pause时候是不执行的
void EventSolve(int key) {
    if (IsSystemPausing == false) {
        player1.direct = 0;
        if (key & MoveRight && xpos1 <= Width - Length + 30) {
            xpos1 += 15;
            player1.direct = 1;
        }
        if (key & MoveLeft && xpos1 > 0) {
            xpos1 -= 15;
            player1.direct = -1;
        }
        if (key & MoveUp && ypos1 > Height / 1.35)
            ypos1 -= 6;
        if (key & MoveDown && ypos1 < Height - 10)
            ypos1 += 6;
        if (key & Shoot) {
            for (int i = 0; i < ExistingBalls; i++)
                if (MyBall[i].collide)shoot(i);
        }
        if (key & FireBoard) {
            P1Board = 1;
            player1.ele = fire;
        }
        if (key & WaterBoard) {
            P1Board = 2;
            player1.ele = water;
        }
        if (key & IceBoard) {
            P1Board = 3;
            player1.ele = ice;
        }
        if (key & RockBoard) {
            P1Board = 4;
            player1.ele = rock;
        }
        if (key & GrassBoard) {
            P1Board = 5;
            player1.ele = grass;
        }
        if (key & WindBoard) {
            P1Board = 6;
            player1.ele = wind;
        }
        if (key & ThunderBoard) {
            P1Board = 7;
            player1.ele = thunder;
        }
    }
}

//EventSolve的更多是板子的运动所以可以停止而不影响pause前后的变化
void update() {
    player1.BoardDesRect.x = xpos1;
    player1.BoardDesRect.y = ypos1;
    if (IsSystemPausing == false) {
        for (int j = 0; j < ExistingBalls; j++)
            BallPlaceUpdate(j);//针对多球的操作
        //在pause的时候球不动所以不能更新位置
    }
    sprintf(score, "Your score is %d", MyScore);
    SDL_DestroyTexture(cTexture);
    cTexture = allTexmanager(score, false, white);
    ScoreRect.x = Width - strlen(score) * 15 - 5;
    ScoreRect.w = strlen(score) * 15;
    if (IsCopyClear())
        StructCopy(testing);
    //施工处，画些新地图
}

//render中实际变化的内容有：Board的位置，Ball的位置，score，清空clock，归零时间（先写暂停函数再说），重置copyq
void render() {
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, BackIMG[1], NULL, &RectBG);
    SDL_RenderCopy(Renderer, Board[P1Board], NULL, &player1.BoardDesRect);
    for (int i = 0; i < ExistingBalls; i++)
        SDL_RenderCopy(Renderer, Ball[MyBall[i].Ball_ele], NULL, &MyBall[i].Ball_rect);
    DrawShield();
    SDL_RenderCopy(Renderer, cTexture, NULL, &ScoreRect);
    ReactionClockCheck();
    printf("%d  %d  %d\n",now,game_start,PauseTime);
    TimerPrint();
    DrawBricks(copyq);
    if (IsSystemPausing)SDL_RenderCopy(Renderer, Pause_and_Restart, NULL, &PAUSERect);
    SDL_RenderPresent(Renderer);
}

void TRANS() {
    SDL_RenderClear(Renderer);
    present(1);
    for (int i = 1; i <= 400; i++) {
        SDL_PollEvent(&Event);
        SDL_Delay(1);
    }
    for (int i = 2; i <= 5; i++) {
        present(i);

        for (int j = 1; j <= 800; j++) {
            SDL_PollEvent(&Event);
            SDL_Delay(1);
        }
    }
    present(6);
    for (int i = 1; i <= 1400; i++) {
        SDL_PollEvent(&Event);
        SDL_Delay(1);
    }
    present(7);
    for (int i = 1; i <= 400; i++) {
        SDL_PollEvent(&Event);
        SDL_Delay(1);
    }
}

void present(int i) {
    char s[40];
    sprintf(s, "media/image/trans%d.png", i);
    SDL_Texture *tmp = TexManager(s);
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, tmp, NULL, &RectBG);
    SDL_DestroyTexture(tmp);
    SDL_RenderPresent(Renderer);
}

void AudioBroadcasting(const char *s, int i) {
    BGM[i] = Mix_LoadMUS(s);
}

void Ignore() {
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
    SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
    SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
    SDL_EventState(SDL_KEYUP, SDL_IGNORE);
}

void Revive() {
    SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
    SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_ENABLE);
    SDL_EventState(SDL_MOUSEBUTTONUP, SDL_ENABLE);
    SDL_EventState(SDL_KEYUP, SDL_ENABLE);
}

void PutSettingPage() {
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, Setpage, NULL, &RectBG);
    SDL_RenderCopy(Renderer, SetButton, NULL, &ButtRect);
    SDL_RenderCopy(Renderer, HelpButton, NULL, &HelpRect);
    SDL_RenderCopy(Renderer, BGbutton, NULL, &ButtonRect);
    SDL_RenderPresent(Renderer);
}

void PutHelpPage() {
    SDL_RenderClear(Renderer);
    SDL_Rect TEXTRect1 = {100, 100, 800, 40}, TEXTRect2 = {100, 150, 17 * 40, 40};
    SDL_Rect TEXTRect3 = {100, 200, 18 * 40, 40};
    SDL_RenderCopy(Renderer, BackIMG[3], NULL, &RectBG);
    SDL_RenderCopy(Renderer, TextALot[0], NULL, &TEXTRect1);
    SDL_RenderCopy(Renderer, TextALot[1], NULL, &TEXTRect2);
    SDL_RenderCopy(Renderer, TextALot[2], NULL, &TEXTRect3);
    SDL_RenderCopy(Renderer, Cross, NULL, &CrossRect);
    SDL_RenderPresent(Renderer);
}

void PutSetPage() {
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, BackIMG[4], NULL, &RectBG);
    SDL_RenderCopy(Renderer, Cross, NULL, &CrossRect);
    SDL_RenderPresent(Renderer);
}


void TimerPrint() {
    if (!IsSystemPausing)
        now = clock();
    if (TimeFlag && !IsSystemPausing) {
        edd = clock();
        if(stt!=0)PauseTime += edd - stt;
        TimeFlag = false;
    }
    if (!TimeFlag && IsSystemPausing) {
        TimeFlag = true;
        stt = clock();
    }
    char *s = malloc(15 * sizeof(char));
    SDL_Rect ClockRect = {0, Height - 25, 0, 20};
    int a = (now - game_start - PauseTime) / CLK_TCK;
    a = 180 - a;
    sprintf(s, "Time left:%02d:%02d", a / 60, a % 60);
    ClockRect.w = strlen(s) * 15;
    GetTime = allTexmanager(s, false, white);
    SDL_RenderCopy(Renderer, GetTime, NULL, &ClockRect);
    SDL_DestroyTexture(GetTime);
}

//render中实际变化的内容有：Board的位置，Ball的位置，score，清空clock，归零时间（先写暂停函数再说√），重置copyq
void RestartGame() {

    SetPlayer();
    PauseTime=0;
    edd=stt=0;
    ReactionNow = 0;
    ExistingBalls = 0;
    CreateBall();
    MyScore = 0;
    StructCopy(hello);
    Mix_HaltMusic();
    Mix_PlayMusic(BGM[2], -1);
    game_start=clock();
    now=clock();

}