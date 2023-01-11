//
// Created by shenjiawei on 2023/1/11.
//

#ifndef WINTER_PLAY_TOOLS_H
#define WINTER_PLAY_TOOLS_H

SDL_Texture *TexManager(const char *texture) {
    SDL_Surface *TempSurface = IMG_Load(texture);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(Renderer, TempSurface);
    SDL_FreeSurface(TempSurface);
    return tex;
}

SDL_Texture *allTexmanager(const char *TTF, bool Chinese, enum TexColor colour) {
    SDL_Surface *words;
    if (Chinese) {
        switch (colour) {
            case black:
                words = TTF_RenderUTF8_Blended(ChineseFont, TTF, blackTEX);
                break;
            case white:
                words = TTF_RenderUTF8_Blended(ChineseFont, TTF, whiteTEX);
                break;
            case yellow:
                words = TTF_RenderUTF8_Blended(ChineseFont, TTF, yellowTEX);
                break;
            case green:
                words = TTF_RenderUTF8_Blended(ChineseFont, TTF, greenTEX);
                break;
            case purple:
                words = TTF_RenderUTF8_Blended(ChineseFont, TTF, purpleTEX);
                break;
            case red:
                words = TTF_RenderUTF8_Blended(ChineseFont,TTF,redTEX);
                break;
        }

    } else {
        switch (colour) {
            case black:
                words = TTF_RenderUTF8_Blended(LargeFont, TTF, blackTEX);
                break;
            case white:
                words = TTF_RenderUTF8_Blended(LargeFont, TTF, whiteTEX);
                break;
            case yellow:
                words = TTF_RenderUTF8_Blended(LargeFont, TTF, yellowTEX);
                break;
            case green:
                words = TTF_RenderUTF8_Blended(LargeFont, TTF, greenTEX);
                break;
            case purple:
                words = TTF_RenderUTF8_Blended(LargeFont, TTF, purpleTEX);
                break;
            case red:
                words = TTF_RenderUTF8_Blended(LargeFont, TTF, redTEX);
                break;
        }
    }
    SDL_Texture *tex = SDL_CreateTextureFromSurface(Renderer, words);
    SDL_FreeSurface(words);
    return tex;
}
int GetKey(const Uint8 *Key) {
    int condition = 0;
    if (Key[SDL_SCANCODE_RIGHT] || Key[SDL_SCANCODE_D])
        condition |= MoveRight;
    if (Key[SDL_SCANCODE_LEFT] || Key[SDL_SCANCODE_A]) {
        condition |= MoveLeft;
    }
    if (Key[SDL_SCANCODE_UP] || Key[SDL_SCANCODE_W])
        condition |= MoveUp;
    if (Key[SDL_SCANCODE_DOWN] || Key[SDL_SCANCODE_S])
        condition |= MoveDown;
    if (Key[SDL_SCANCODE_SPACE])
        condition |= Shoot;
    if (Key[SDL_SCANCODE_ESCAPE])
        IsGameRunning = false;
    if (Key[SDL_SCANCODE_1])
        condition |= FireBoard;
    if (Key[SDL_SCANCODE_2])
        condition |= WaterBoard;
    if (Key[SDL_SCANCODE_3])
        condition |= IceBoard;
    if (Key[SDL_SCANCODE_4])
        condition |= RockBoard;
    if (Key[SDL_SCANCODE_5])
        condition |= GrassBoard;
    if (Key[SDL_SCANCODE_6])
        condition |= WindBoard;
    if (Key[SDL_SCANCODE_7])
        condition |= ThunderBoard;
    return condition;
}

bool IsClicked(SDL_Rect Rect, SDL_Event event) {
    if (event.button.x > Rect.x && event.button.x < Rect.x + Rect.w
        && event.button.y > Rect.y && event.button.y < Rect.y + Rect.h)
        return true;
    else return false;
}

#endif //WINTER_PLAY_TOOLS_H
