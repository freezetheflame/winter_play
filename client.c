#include"winter_playh.h"
#include"board.h"
#include "Ball.h"
#include "map.h"
#include "tools.h"
#include "socket_client.h"

int main() {
    g_hMutex = CreateMutex(NULL, FALSE, NULL);
    const Uint8 *KeyValue = SDL_GetKeyboardState(NULL);
    INITALL();
    LOADING();
    nownode = new;
    SDL_Event event;
    if (Mix_PausedMusic())
        Mix_ResumeMusic();
    else
        Mix_PlayMusic(BGM[1], -1);
    while (IsNotGameStarting) {
        //printf("I'm coming!\n");
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
    //printf("I've come here\n");
    Ignore();
    //TRANS();
    Revive();
    do {
        hesitation = false;
        while (IsSettingGoing) {
            if (!setting && !helping)PutSettingPage();
            SDL_PollEvent(&Event);
            if (helping)PutHelpPage();
            if (setting)PutSetPage();
            if (Event.type == SDL_MOUSEBUTTONDOWN && IsClicked(ButtonRect, Event) && !setting && !helping)
                IsSettingGoing = false;
            else if (Event.type == SDL_MOUSEBUTTONDOWN && IsClicked(NetRect, Event) && !setting && !helping) {
                if (!IsMultiplayer)IsMultiplayer = true;
                else IsMultiplayer = false;
            } else if (Event.type == SDL_QUIT) {
                QUIT();
                return 0;
            } else if (Event.type == SDL_KEYDOWN || Event.type == SDL_MOUSEBUTTONDOWN) {
                if ((Event.key.keysym.sym == SDLK_h && !setting) || (!helping && IsClicked(HelpRect, Event)) ||
                    (helping && IsClicked(CrossRect, Event))) {
                    helping = (helping == true ? false : true);
                } else if ((Event.key.keysym.sym == SDLK_s && !helping) || (!setting && IsClicked(ButtRect, Event)) ||
                           (setting &&
                            IsClicked(CrossRect, Event))) {
                    setting = (setting == true ? false : true);
                }
            }
        }
        if (IsMultiplayer) {
            StartUpSocket();
            CreateThread(NULL, (SIZE_T) NULL, (LPTHREAD_START_ROUTINE) maintainContact, NULL, (DWORD) 0UL, NULL);
            request_queue = malloc(sizeof(node));
            request_queue->next = NULL;
        }
        while (ReadyNot) {
            WaitForSingleObject(g_hMutex, INFINITE);
            if (!IsMultiplayer)ReadyNot = false;
            SDL_PollEvent(&Event);
            switch (Event.type) {
                case SDL_QUIT:
                    IsGameRunning = false;
                    ReadyNot = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (IsClicked(NetExitRect, Event)) {
                        ReadyNot = false;
                        hesitation = true;
                        IsSettingGoing = true;
                        closesocket(server_socket);
                        WSACleanup();
                    }
                    break;
            }
            SDL_RenderClear(Renderer);
            SDL_RenderCopy(Renderer, loading, NULL, &loadingRect);
            SDL_RenderCopy(Renderer, ExitingNet, NULL, &NetExitRect);
            SDL_RenderPresent(Renderer);
            ReleaseMutex(g_hMutex);
        }
    } while (hesitation);
    //printf("I've come there\n");
    timehello = 1;
    if (IsMultiplayer)CreateBall(2);
    else CreateBall(1);
    Mix_PlayMusic(BGM[2], -1);
    Mix_ResumeMusic();
    while (IsGameRunning) {
        frameStart = SDL_GetTicks();
        WaitForSingleObject(g_hMutex, INFINITE);
        //process the input
        SDL_PumpEvents();
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            QUIT();
            break;
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_p && !IsMultiplayer) {
                IsSystemPausing = IsSystemPausing == true ? false : true;
                printf("place1 found with error");
                if (Mix_PausedMusic())Mix_ResumeMusic();
                else Mix_PauseMusic();
            }
        }
        //if (event.type == SDL_MOUSEBUTTONDOWN)printf("(%d,%d)\n", event.button.x, event.button.y);
        if (IsSystemPausing) {
            if (event.type == SDL_MOUSEBUTTONDOWN && IsClicked(ContinueRect, event) && !IsGameEnding) {
                //printf("place2");
                IsSystemPausing = false;
                Mix_ResumeMusic();
            } else if (event.type == SDL_MOUSEBUTTONDOWN && IsClicked(EscRect, event) && !IsGameEnding) {
                printf("Someone exit\n");
                IsGameEnding = true;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && IsClicked(RestartRect, event) && !IsGameEnding) {

                Mix_PlayChannel(-1, chunks[5], 0);
                for (int j = 1; j <= 2000; j++) {
                    SDL_PollEvent(&Event);
                    SDL_Delay(1);
                }
                RestartGame();
                IsSystemPausing = false;
            }
        }
        if (IsGameEnding) {
            if (!Mix_PausedMusic())Mix_PauseMusic();
            IsSystemPausing = true;
            if (event.type == SDL_MOUSEBUTTONDOWN && IsClicked(GEEsc, event)) {
                IsGameRunning = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && IsClicked(GERestart, event) && !IsMultiplayer) {
                Mix_PlayChannel(-1, chunks[5], 0);
                for (int j = 1; j <= 1500; j++) {
                    SDL_PollEvent(&Event);
                    SDL_Delay(1);
                }
                RestartGame();
                IsSystemPausing = false;
                IsGameRunning = true;
                IsGameEnding = false;
            }
        }
        if (client_num == 1 && IsMultiplayer) {
            EventSolve_for_P2(HisKey);
            EventSolve_for_P1(GetKey(KeyValue));
        } else if (IsMultiplayer) {
            EventSolve_for_P1(HisKey);
            EventSolve_for_P2(GetKey(KeyValue));
        } else {
            EventSolve_for_P1(GetKey(KeyValue));
        }
        if (IsMultiplayer) {
            WaitForSingleObject(g_hMutex, INFINITE);
            node *request = malloc(sizeof(node));
            memset(request->buf, '\0', sizeof(request->buf));
            request->next = NULL;
            sprintf(request->buf, "%d", MyKeyboard);
            getLastElement(request_queue)->next = request;
            ReleaseMutex(g_hMutex);
        }
        if (timehello)game_start = clock();
        timehello = 0;
        update();
        render();
        frameTime = (int) SDL_GetTicks() - (int) frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
        ReleaseMutex(g_hMutex);
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
    MapInitial();

    printf("%d", new->arr[1][1]);
    StructCopy(new->arr);
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
    chunks[5] = Mix_LoadWAV("media/audio/Restart.mp3");
    chunks[6] = Mix_LoadWAV("media/audio/ROCK1.mp3");
    Shield = TexManager("media/image/sheild.png");
    GameEnding = TexManager("media/image/GameEnding.png");
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
    TextALot[3]= allTexmanager("细节详见附加Markdown文档",true,white);
    Pause_and_Restart = TexManager("media/image/PauseandRestart.png");
    NetButton[0] = TexManager("media/image/buttonoff.png");
    NetButton[1] = TexManager("media/image/buttonon.png");
    loading = allTexmanager("LOADING...", false, white);
    ExitingNet = TexManager("media/image/ExitingNet.png");
    gift = TexManager("media/image/gift.png");
    NetTEXT = allTexmanager("联机模式", true, white);
    IsGameRunning = true;
    IsNotGameStarting = true;
    IsSettingGoing = true;
    SetPlayer();
}

void QUIT() {
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    SDL_Quit();
}

//当pause时候是不执行的
void EventSolve_for_P1(int key) {
    if (IsSystemPausing == false) {
        player1.direct = 0;
        if (key & QDoneAll && Qusage1 == 1) {
            Qusage1--;
            DoneALL();
        }
        if (key & MoveRight && xpos1 <= Width - Length + 30) {
            xpos1 += 15 * speedset;
            player1.direct = 1;
        }
        if (key & MoveLeft && xpos1 > 0) {
            xpos1 -= 15 * speedset;
            player1.direct = -1;
        }
        if (key & MoveUp && ypos1 > Height / 1.35)
            ypos1 -= 6 * speedset;
        if (key & MoveDown && ypos1 < Height - 10)
            ypos1 += 6 * speedset;
        if (key & Shoot) {
            for (int i = 0; i < ExistingBalls; i++)
                if (MyBall[i].collide)shoot_of_P1(i);
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

void EventSolve_for_P2(int key) {
    if (IsSystemPausing == false) {
        player2.direct = 0;
        if (key & QDoneAll && Qusage2 == 1) {
            Qusage2--;
            DoneALL();
        }
        if (key & MoveRight && xpos2 <= Width - Length + 30) {
            xpos2 += 15 * speedset;
            player2.direct = 1;
        }
        if (key & MoveLeft && xpos2 > 0) {
            xpos2 -= 15 * speedset;
            player2.direct = -1;
        }
        if (key & MoveUp && ypos2 > Height / 1.35)
            ypos2 -= 6 * speedset;
        if (key & MoveDown && ypos2 < Height - 10)
            ypos2 += 6 * speedset;
        if (key & Shoot) {
            for (int i = 0; i < ExistingBalls; i++)
                if (MyBall[i].collide)shoot_of_P2(i);
        }
        if (key & FireBoard) {
            P2Board = 1;
            player2.ele = fire;
        }
        if (key & WaterBoard) {
            P2Board = 2;
            player2.ele = water;
        }
        if (key & IceBoard) {
            P2Board = 3;
            player2.ele = ice;
        }
        if (key & RockBoard) {
            P2Board = 4;
            player2.ele = rock;
        }
        if (key & GrassBoard) {
            P2Board = 5;
            player2.ele = grass;
        }
        if (key & WindBoard) {
            P2Board = 6;
            player2.ele = wind;
        }
        if (key & ThunderBoard) {
            P2Board = 7;
            player2.ele = thunder;
        }
    }
}

//EventSolve的更多是板子的运动所以可以停止而不影响pause前后的变化
void update() {
    player1.BoardDesRect.x = xpos1;
    player1.BoardDesRect.y = ypos1;
    player2.BoardDesRect.x = xpos2;
    player2.BoardDesRect.y = ypos2;
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
    if (IsCopyClear() && Mapcount <= 3) {
        Mapcount++;
        nownode = nownode->next;
        StructCopy(nownode->arr);
    } else if (Mapcount > 3 && IsCopyClear())
        IsGameEnding = true;
    //HisKey=0;
    //施工处，画些新地图
}

void render() {
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, BackIMG[1], NULL, &RectBG);
    SDL_RenderCopy(Renderer, Board[P1Board], NULL, &player1.BoardDesRect);
    if (IsMultiplayer)
        SDL_RenderCopy(Renderer, Board[P2Board], NULL, &player2.BoardDesRect);
    for (int i = 0; i < ExistingBalls; i++)
        SDL_RenderCopy(Renderer, Ball[MyBall[i].Ball_ele], NULL, &MyBall[i].Ball_rect);
    DrawShield();
    SDL_RenderCopy(Renderer, cTexture, NULL, &ScoreRect);
    SDL_DestroyTexture(cTexture);
    ReactionClockCheck();
    TimerPrint();
    DrawBricks(copyq);
    if (IsSystemPausing) {
        SDL_RenderCopy(Renderer, Pause_and_Restart, NULL, &PAUSERect);
        //printf("OhMyGod");
    }
    if (IsGameEnding) {
        SDL_RenderCopy(Renderer, GameEnding, NULL, &GameEndingRect);
        SDL_Texture *MyText, *TheScore;
        char *s = malloc(6 * sizeof(char));
        sprintf(s, "%d", MyScore);
        MyText = allTexmanager("Your Final Score is", false, black);
        if (MyScore <= 100)TheScore = allTexmanager(s, false, black);
        else if (MyScore <= 200)TheScore = allTexmanager(s, false, green);
        else if (MyScore <= 500)TheScore = allTexmanager(s, false, yellow);
        else TheScore = allTexmanager(s, false, red);
        GEText2.w = strlen(s) * GEText2.h;
        free(s);
        SDL_RenderCopy(Renderer, MyText, NULL, &GEText1);
        SDL_RenderCopy(Renderer, TheScore, NULL, &GEText2);
        SDL_DestroyTexture(MyText);
        SDL_DestroyTexture(TheScore);
    }
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
    SDL_RenderCopy(Renderer,NetTEXT,NULL,&NetTEXTRect);
    if (!IsMultiplayer)SDL_RenderCopy(Renderer, NetButton[0], NULL, &NetRect);
    else SDL_RenderCopy(Renderer, NetButton[1], NULL, &NetRect);
    SDL_RenderPresent(Renderer);
}

void PutHelpPage() {
    SDL_RenderClear(Renderer);
    SDL_Rect TEXTRect1 = {100, 100, 800, 40}, TEXTRect2 = {100, 150, 17 * 40, 40};
    SDL_Rect TEXTRect3 = {100, 200, 18 * 40, 40};
    SDL_Rect TEXTRect4 = {100,250,11*40,40};
    SDL_RenderCopy(Renderer, BackIMG[3], NULL, &RectBG);
    SDL_RenderCopy(Renderer, TextALot[0], NULL, &TEXTRect1);
    SDL_RenderCopy(Renderer, TextALot[1], NULL, &TEXTRect2);
    SDL_RenderCopy(Renderer, TextALot[2], NULL, &TEXTRect3);
    SDL_RenderCopy(Renderer,TextALot[3],NULL,&TEXTRect4);
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
        if (stt != 0)PauseTime += edd - stt;
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
    if (a <= 0)IsGameEnding = true;
    sprintf(s, "Time left:%02d:%02d", a / 60, a % 60);
    ClockRect.w = strlen(s) * 15;
    GetTime = allTexmanager(s, false, white);
    SDL_RenderCopy(Renderer, GetTime, NULL, &ClockRect);
    SDL_DestroyTexture(GetTime);
}

//render中实际变化的内容有：Board的位置，Ball的位置，score，清空clock，归零时间（先写暂停函数再说√），重置copyq
void RestartGame() {
    Qusage1 = 1;
    SetPlayer();
    PauseTime = 0;
    edd = stt = 0;
    ReactionNow = 0;
    ExistingBalls = 0;
    CreateBall(1);
    MyScore = 0;
    Mapcount = 0;
    StructCopy(new->arr);
    nownode = new;
    Mix_HaltMusic();
    Mix_PlayMusic(BGM[2], -1);
    game_start = clock();
    now = clock();
}
