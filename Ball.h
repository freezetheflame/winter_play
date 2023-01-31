//
// Created by shenjiawei on 2023/1/4.
//

#ifndef WINTER_PLAY_BALL_H
#define WINTER_PLAY_BALL_H

#include "winter_playh.h"
#include "board.h"
#include <math.h>

typedef struct Ball {
    enum elements Ball_ele;
    SDL_Rect Ball_rect;
    bool collide;
    double Speed_x, Speed_y;
    int master;
} ball;
ball MyBall[50];
int ExistingBalls = 0;

void Need_To_Add_Ball();

void CreateBall(int flag) {
    MyBall[ExistingBalls].Ball_rect.w = MyBall[ExistingBalls].Ball_rect.h = 30;
    MyBall[ExistingBalls].Ball_rect.x = player1.BoardDesRect.x + player1.BoardDesRect.w / 2;
    MyBall[ExistingBalls].Ball_rect.y = player1.BoardDesRect.y - MyBall[ExistingBalls].Ball_rect.w;
    MyBall[ExistingBalls].collide = true;
    MyBall[ExistingBalls].Speed_y = -8 * ballset;
    MyBall[ExistingBalls].Speed_x = -3 * ballset;
    MyBall[ExistingBalls].Ball_ele = fire;
    MyBall[ExistingBalls].master = 1;
    ExistingBalls++;
    if (IsMultiplayer && flag == 2) {
        MyBall[ExistingBalls].Ball_rect.w = MyBall[ExistingBalls].Ball_rect.h = 30;
        MyBall[ExistingBalls].Ball_rect.x = player2.BoardDesRect.x + player1.BoardDesRect.w / 2;
        MyBall[ExistingBalls].Ball_rect.y = player2.BoardDesRect.y - MyBall[ExistingBalls].Ball_rect.w;
        MyBall[ExistingBalls].collide = true;
        MyBall[ExistingBalls].Speed_y = -8 * ballset;
        MyBall[ExistingBalls].Speed_x = -3 * ballset;
        MyBall[ExistingBalls].Ball_ele = fire;
        MyBall[ExistingBalls].master = 2;
        ExistingBalls++;
    }
}

void setClock(bool *hello, double time, int i) {
    MyTime[i].start = clock();
    MyTime[i].time = time;
    MyTime[i].flag = hello;
}

bool checkClock(int i) {
    now = clock();
    if ((now - MyTime[i].start) / CLK_TCK >= MyTime[i].time && MyTime[i].flag != NULL) {
        *MyTime[i].flag = false;
        return true;
    }
    return false;
}

void DestroyBall(int i) {
    if (i != ExistingBalls)//不是最后一个球消失，那么转移财产
    {
        MyBall[i].Ball_ele = MyBall[ExistingBalls].Ball_ele;
        MyBall[i].Ball_rect = MyBall[ExistingBalls].Ball_rect;
        MyBall[i].Speed_x = MyBall[ExistingBalls].Speed_x;
        MyBall[i].Speed_y = MyBall[ExistingBalls].Speed_y;
        MyBall[i].collide = MyBall[ExistingBalls].collide;
    }
}

void WallCrushingJudge(int i) {
    if ((MyBall[i].Ball_rect.x <= 0 && MyBall[i].Speed_x < 0) ||
        (MyBall[i].Ball_rect.x >= Width && MyBall[i].Speed_x > 0))
        MyBall[i].Speed_x *= -1;
    if (MyBall[i].Ball_rect.y <= 0 && MyBall[i].Speed_y < 0)
        MyBall[i].Speed_y *= -1;
    if (MyBall[i].Ball_rect.y >= Height - 10 && !Skill_of_Rock) {
        if (ExistingBalls > 1) {
            ExistingBalls--;
            DisappearBalls++;
            DestroyBall(i);
        } else {
            ExistingBalls--;
            DisappearBalls++;
            DestroyBall(i);
            CreateBall(2);
        }
    } else if (MyBall[i].Ball_rect.y >= Height - 30 && Skill_of_Rock) {
        MyBall[i].Speed_y *= -1;

    }

}

int bumpJudging(enum elements a, enum elements b) {
    if (a == b)return 0;//0对应的是不反应
    else
        switch (a) {
            case fire:
                switch (b) {
                    case ice:
                        return 1;
                    case water:
                        return 1;//1对应的是增幅反应
                    case grass:
                        return 2;//燃烧反应，实现不实现看我心情
                    case rock:
                        return 7;//开盾
                    case thunder:
                        return 3;//爆炸
                    case wind:
                        return 4;//扩散染色反应
                    case fire:
                        break;
                }
                break;
            case water:
                switch (b) {
                    case fire:
                        return 1;
                    case ice:
                        return 5;//串联反应
                    case thunder:
                        return 5;
                    case grass:
                        return 3;//草种子爆炸嘛
                    case wind:
                        return 4;
                    case rock:
                        return 7;
                    case water:
                        break;
                }
                break;
            case ice:
                switch (b) {
                    case fire:
                        return 1;
                    case water:
                        return 5;//串联反应
                    case thunder:
                        return 5;
                    case grass:
                        return 0;
                    case wind:
                        return 4;
                    case rock:
                        return 7;
                    case ice:
                        break;
                }
                break;
            case rock:
                return 7;
            case grass:
                switch (b) {
                    case fire:
                        return 2;
                    case ice:
                        return 0;//冰草不反应
                    case thunder:
                        return 1;//增幅
                    case water:
                        return 3;//草种子爆炸嘛
                    case wind:
                        return 4;
                    case rock:
                        return 7;
                    case grass:
                        break;
                }
                break;
            case wind:
                if (b != rock)return 6;
                else return 7;
            case thunder:
                switch (b) {
                    case fire:
                        return 3;
                    case ice:
                        return 5;//串联反应
                    case thunder:
                        return 0;
                    case grass:
                        return 1;//激化
                    case wind:
                        return 4;
                    case rock:
                        return 7;
                    case water:
                        return 5;
                }
        }
}

void spread(int row, int column, int element) {
    for (int j = row - 1; j <= row + 1; j++)
        for (int p = column - 1; p <= column + 1; p++) {
            if (j != row || p != column && j >= 0 && p >= 0 && p < 20) {
                if (copyq[j][p] > 0 && copyq[j][p] < 10 && copyq[j][p] != element) {
                    MyScore++;
                    copyq[j][p] = 0;
                } else if (copyq[j][p] >= 10 && copyq[j][p] < 100 && copyq[j][p] != element * 10) {
                    copyq[j][p] = element;
                    MyScore++;
                } else if (copyq[j][p] >= 100 && copyq[j][p] != element * 100) {
                    MyScore++;
                    copyq[j][p] = element * 10;
                }
            }
        }
    copyq[row][column] /= 10;
}

void connection(int row, int column, int ele, int place) {
    copyq[row][column] /= 10;
    flagtemp[row][column] = 1;
    MyScore++;
    if ((copyq[row + 1][column] == ele || copyq[row + 1][column] == ele * 10 || copyq[row + 1][column] == ele * 100) &&
        (place != 1) && !flagtemp[row + 1][column])
        connection(row + 1, column, ele, 2);
    if ((row - 1 >= 0) &&
        (copyq[row - 1][column] == ele || copyq[row - 1][column] == ele * 10 || copyq[row - 1][column] == ele * 100) &&
        (place != 2) && !flagtemp[row - 1][column])
        connection(row - 1, column, ele, 1);
    if ((copyq[row][column + 1] == ele || copyq[row][column + 1] == ele * 10 || copyq[row][column + 1] == ele * 100) &&
        (place != 3) && !flagtemp[row][column + 1])
        connection(row, column + 1, ele, 4);
    if ((column - 1 >= 0) &&
        (copyq[row][column - 1] == ele || copyq[row][column - 1] == ele * 10 || copyq[row][column - 1] == ele * 100) &&
        (place != 4) && !flagtemp[row][column - 1])
        connection(row, column - 1, ele, 3);

}

void ElementReaction(int i, int row, int column, int Ballnum) {
    switch (i) {
        case 1:
            copyq[row][column] /= 100;
            MyScore += 2;
            break;
        case 2:
            copyq[row][column] /= 100;//燃烧还不会写，反应文字可以不一样
            MyScore += 2;
            break;
        case 3:
            if (row >= 1) {
                if (column >= 1)copyq[row - 1][column - 1] /= 10;
                copyq[row - 1][column] /= 10;
                copyq[row - 1][column + 1] /= 10;
            }
            if (column >= 1) copyq[row][column - 1] /= 10;
            copyq[row][column] /= 100;
            copyq[row][column + 1] /= 10;
            if (column >= 1)copyq[row + 1][column - 1] /= 10;
            copyq[row + 1][column] /= 10;
            copyq[row + 1][column + 1] /= 10;
            MyScore += 9;
            break;
        case 4:
            tmp1 = MyBall[i].Ball_ele;
            spread(row, column, tmp1);
            copyq[row][column] /= 10;
            break;
        case 5:
            tmp1 = copyq[row][column];
            while (tmp1 >= 10)tmp1 /= 10;
            memset(flagtemp, 0, sizeof(flagtemp));
            connection(row, column, tmp1, 0);
            break;
        case 6:
            tmp1 = copyq[row][column];
            while (tmp1 >= 10)tmp1 /= 10;
            spread(row, column, tmp1);
            break;
        case 7:
            MyScore++;
            if (!Skill_of_Rock) {
                Skill_of_Rock = true;
                setClock(&Skill_of_Rock, 15, 1);
            }
            if (!Is_Sound_Skill_Of_Rock_Going) {
                Mix_PlayChannel(-1, chunks[zhongli], 0);
                zhongli++;
                if (zhongli >= 5)zhongli = 2;
                Is_Sound_Skill_Of_Rock_Going = true;
                MyTime[1].start = clock();//续盾
                setClock(&Is_Sound_Skill_Of_Rock_Going, 10, 2);
            }
            copyq[row][column] /= 10;
            break;
        case 0:
            copyq[row][column] /= 10;
            MyScore++;
            break;
        default:
            break;
    }
}

int IsRect(int x1, int y1, int x2, int y2, int l1, int w1, int l2, int w2) {
    int x3 = x1 + l1, y3 = y1 + w1, x4 = x2 + l2, y4 = w2 + y2;
    int bigl = max(abs(x1 - x4), abs(x2 - x3));
    int bigw = max(abs(y1 - y4), abs(y2 - y3));
    if (bigl - l1 - l2 <= 0 && bigw - w1 - w2 <= 0)
        return 1;
    else return 0;
}

//矩形碰撞框判断函数
//have support multi_player
void BoardCrushingJudge(int i) {
    if (MyBall[i].Speed_y > 0 &&
        IsRect(MyBall[i].Ball_rect.x, MyBall[i].Ball_rect.y, player1.BoardDesRect.x, player1.BoardDesRect.y,
               MyBall[i].Ball_rect.w, MyBall[i].Ball_rect.w, player1.BoardDesRect.w, player1.BoardDesRect.h)
            ) {
        MyBall[i].Speed_y *= -1;
        MyBall[i].Ball_ele = player1.ele;
        if (fabs(MyBall[i].Speed_x) < 6 || MyBall[i].Speed_x * player1.direct < 0) {
            MyBall[i].Speed_x += player1.direct * 3.8;
            //printf("BALL%dCrushBoard\n", i);
        }


    } else if (IsMultiplayer && MyBall[i].Speed_y > 0 &&
               IsRect(MyBall[i].Ball_rect.x, MyBall[i].Ball_rect.y, player2.BoardDesRect.x, player2.BoardDesRect.y,
                      MyBall[i].Ball_rect.w, MyBall[i].Ball_rect.w, player1.BoardDesRect.w, player1.BoardDesRect.h)) {
        MyBall[i].Speed_y *= -1;
        MyBall[i].Ball_ele = player1.ele;
        if (fabs(MyBall[i].Speed_x) < 6 || MyBall[i].Speed_x * player2.direct < 0) {
            MyBall[i].Speed_x += player2.direct * 3.8;
        }
    }
    Need_To_Add_Ball();
}

void BrickCrushingJudge(int i) {
    double x1 = MyBall[i].Ball_rect.x, y1 = MyBall[i].Ball_rect.y;
    double x3 = x1 - MyBall[i].Speed_x, y3 = y1 - MyBall[i].Speed_y;
    int x2, y2;
    for (int row = 0; row < 15; row++)
        for (int column = 0; column < 25; column++) {
            if (copyq[row][column] != 0) {
                x2 = column * 48;
                y2 = row * 48;
                if (IsRect(x1, y1, x2, y2, MyBall[i].Ball_rect.w, MyBall[i].Ball_rect.h, 48, 48)) {
                    int tmp;
                    if (copyq[row][column] >= 10) {
                        tmp = copyq[row][column] / 10;
                        if (tmp >= 10)tmp /= 10;
                    } else tmp = copyq[row][column];
                    int judge = bumpJudging(MyBall[i].Ball_ele, tmp);
                    ElementReaction(judge, row, column, i);
                    switch (judge) {
                        case 1:
                            ReactionPV(increase, MyBall[i].Ball_rect);
                            break;
                        case 2:
                            ReactionPV(burning, MyBall[i].Ball_rect);
                            break;
                        case 3:
                            ReactionPV(booming, MyBall[i].Ball_rect);
                            break;
                        case 4:
                            ReactionPV(spreade, MyBall[i].Ball_rect);
                            break;
                        case 5:
                            ReactionPV(linking, MyBall[i].Ball_rect);
                            break;
                        case 6:
                            ReactionPV(spreade, MyBall[i].Ball_rect);
                            break;
                        case 7:
                            ReactionPV(crystal, MyBall[i].Ball_rect);
                            break;
                        default:
                            break;
                    }
                    if (tmp <= 7) {
                        if (x1 < x2) {
                            if (y1 < y2) {
                                //printf("1");
                                if ((double) (x2 - x3) / fabs(MyBall[i].Speed_x) <
                                    (double) (y2 - y3) / fabs(MyBall[i].Speed_y))
                                    MyBall[i].Speed_y *= -1;
                                else if ((double) (x2 - x3) / fabs(MyBall[i].Speed_x) >
                                         (double) (y2 - y3) / fabs(MyBall[i].Speed_y))
                                    MyBall[i].Speed_x *= -1;
                                else {
                                    MyBall[i].Speed_y *= -1;
                                    MyBall[i].Speed_x *= -1;
                                }

                            } else {
                                //printf("2");
                                if ((double) (x2 - x3) / fabs(MyBall[i].Speed_x) <
                                    (double) (y3 - y2) / fabs(MyBall[i].Speed_y))
                                    MyBall[i].Speed_y *= -1;
                                else {
                                    if ((double) (x2 - x3) / fabs(MyBall[i].Speed_x) >
                                        (double) (y3 - y2) / fabs(MyBall[i].Speed_y))
                                        MyBall[i].Speed_x *= -1;

                                    else {
                                        MyBall[i].Speed_y *= -1;
                                        MyBall[i].Speed_x *= -1;
                                    }
                                }
                            }
                        } else {
                            if (y1 < y2) {
                                //printf("3");
                                if ((double) (x3 - x2) / fabs(MyBall[i].Speed_x) <
                                    (double) (y2 - y3) / fabs(MyBall[i].Speed_y))
                                    MyBall[i].Speed_y *= -1;
                                else if ((double) (x3 - x2) / fabs(MyBall[i].Speed_x) >
                                         (double) (y2 - y3) / fabs(MyBall[i].Speed_y))
                                    MyBall[i].Speed_x *= -1;
                                else {
                                    MyBall[i].Speed_y *= -1;
                                    MyBall[i].Speed_x *= -1;
                                }
                            } else {
                                //printf("4");
                                if ((double) (x3 - x2) / fabs(MyBall[i].Speed_x) <
                                    (double) (y3 - y2) / fabs(MyBall[i].Speed_y))
                                    MyBall[i].Speed_y *= -1;
                                else if ((double) (x3 - x2) / fabs(MyBall[i].Speed_x) >
                                         (double) (y3 - y2) / fabs(MyBall[i].Speed_y))
                                    MyBall[i].Speed_x *= -1;
                                else {
                                    MyBall[i].Speed_y *= -1;
                                    MyBall[i].Speed_x *= -1;
                                }
                            }
                        }

                    } else {
                        if (tmp == 8) {
                            copyq[row][column] = 0;
                            speedset = 2;
                            IsDoubleSpeed = true;
                            setClock(&IsDoubleSpeed, 10, 3);
                        }
                        if (tmp == 9) {
                            copyq[row][column] = 0;
                            player1.BoardDesRect.w *= 2;
                            player2.BoardDesRect.w *= 2;
                            IsDoubleLength = true;
                            setClock(&IsDoubleLength, 10, 4);
                        }
                    }
                }

            }
        }
}


void Ballupdate(int i) {
    WallCrushingJudge(i);
    BoardCrushingJudge(i);
    BrickCrushingJudge(i);
    if (Is_Sound_Skill_Of_Rock_Going || Skill_of_Rock) {
        checkClock(1);
        checkClock(2);
    }
    if (IsDoubleSpeed) {
        checkClock(3);
    }
    if (!IsDoubleSpeed)speedset = 1;
    if (IsDoubleLength) {
        checkClock(4);
    }
    if (!IsDoubleLength) {
        player1.BoardDesRect.w = Length;
        player2.BoardDesRect.w = Length;
    }
    MyBall[i].Ball_rect.x += MyBall[i].Speed_x;
    MyBall[i].Ball_rect.y += MyBall[i].Speed_y;
}

void BallPlaceUpdate(int i) {

    if (MyBall[i].collide == true && MyBall[i].master == 1) {
        MyBall[i].Ball_ele = player1.ele;
        MyBall[i].Ball_rect.x = player1.BoardDesRect.x + player1.BoardDesRect.w / 2;
        MyBall[i].Ball_rect.y = player1.BoardDesRect.y - MyBall[i].Ball_rect.w;
        return;
    } else if (MyBall[i].collide == true && MyBall[i].master == 2) {
        MyBall[i].Ball_ele = player2.ele;
        MyBall[i].Ball_rect.x = player2.BoardDesRect.x + player1.BoardDesRect.w / 2;
        MyBall[i].Ball_rect.y = player2.BoardDesRect.y - MyBall[i].Ball_rect.w;
        return;
    } else Ballupdate(i);

}

void shoot_of_P1(int i) {
    if (MyBall[i].collide == true && MyBall[i].master == 1) {
        MyBall[i].collide = false;
        if (!IsMultiplayer)MyBall[i].Speed_x = player1.direct * rand() % 10 / 3.0;
        else MyBall[i].Speed_x = player1.direct * (3.0) * ballset;
    }
}

void shoot_of_P2(int i) {
    if (MyBall[i].collide == true && MyBall[i].master == 2) {
        MyBall[i].collide = false;
        if (!IsMultiplayer)MyBall[i].Speed_x = player2.direct * rand() % 10 / 3.0;
        else MyBall[i].Speed_x = player2.direct * (3.0) * ballset;
    }
}

void DrawShield() {
    SDL_Rect ShieldRect = {0, Height - 20, Width, 18};
    if (Skill_of_Rock)
        SDL_RenderCopy(Renderer, Shield, NULL, &ShieldRect);
}

void Need_To_Add_Ball() {
    if (MyScore / 60 > ExistingBalls + DisappearBalls - 2) {
        CreateBall(1);
        shoot_of_P1(ExistingBalls - 1);
    }

}

#endif//WINTER_PLAY_BALL_H
