/*
 * Copyright (c) Pavol Rusnak, SatoshiLabs
 *
 * Licensed under Microsoft Reference Source License (Ms-RSL)
 * see LICENSE.md file for details
 */

#include <SDL2/SDL.h>
#include <stdlib.h>

static SDL_Renderer *RENDERER = 0;
static SDL_Surface  *SCREEN   = 0;
static SDL_Texture  *TEXTURE  = 0;
static int DATAODD = 0;
static int POSX, POSY, SX, SY, EX, EY = 0;
static int ROTATION = 0;

#define CMD(X) (void)(X);
#define DATA(X) DATAfunc((X))

#define DISPLAY_BORDER 8

static void DATAfunc(uint8_t x) {
    if (POSX <= EX && POSY <= EY) {
        ((uint8_t *)SCREEN->pixels)[POSX * 2 + POSY * SCREEN->pitch + (DATAODD ^ 1)] = x;
    }
    DATAODD = !DATAODD;
    if (DATAODD == 0) {
        POSX++;
        if (POSX > EX) {
            POSX = SX;
            POSY++;
        }
    }
}

uint32_t trezorui_poll_sdl_event(uint32_t timeout_us)
{
    SDL_Event event;
    int x, y;
    if (SDL_WaitEventTimeout(&event, timeout_us / 1000) > 0) {
        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONUP:
                x = event.button.x - DISPLAY_BORDER;
                y = event.button.y - DISPLAY_BORDER;
                if (x < 0 || y < 0 || x >= RESX || y >= RESY) break;
                switch (event.type) {
                    case SDL_MOUSEBUTTONDOWN:
                        return (0x00 << 24) | (0x01 << 16) | (x << 8) | y; // touch_start
                        break;
                    case SDL_MOUSEMOTION:
                        if (event.motion.state) {
                            return (0x00 << 24) | (0x02 << 16) | (x << 8) | y; // touch_move
                        }
                        break;
                    case SDL_MOUSEBUTTONUP:
                        return (0x00 << 24) | (0x03 << 16) | (x << 8) | y; // touch_end
                        break;
                }
                break;
            case SDL_QUIT:
                exit(3);
                break;
        }
    }
    return 0;
}

static void display_init(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
    }
    SDL_Window *win = SDL_CreateWindow("TREZOR", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, RESX + 2 * DISPLAY_BORDER, RESY + 2 * DISPLAY_BORDER, SDL_WINDOW_SHOWN);
    if (!win) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
    }
    RENDERER = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
    if (!RENDERER) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
    }
    SDL_RenderClear(RENDERER);
    SCREEN = SDL_CreateRGBSurface(0, RESX, RESY, 16, 0xF800, 0x07E0, 0x001F, 0x0000);
    TEXTURE = SDL_CreateTexture(RENDERER, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, RESX, RESY);
}

static void display_set_window(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    SX = x; SY = y;
    EX = x + w - 1; EY = y + h - 1;
    POSX = SX; POSY = SY;
    DATAODD = 0;
}

static void display_update(void)
{
    SDL_RenderClear(RENDERER);
    SDL_UpdateTexture(TEXTURE, NULL, SCREEN->pixels, SCREEN->pitch);
    const SDL_Rect r = {DISPLAY_BORDER, DISPLAY_BORDER, RESX, RESY};
    SDL_RenderCopyEx(RENDERER, TEXTURE, NULL, &r, ROTATION, NULL, 0);
    SDL_RenderPresent(RENDERER);
}

static void display_orientation(int degrees)
{
    ROTATION = degrees;
    display_update();
}

static void display_backlight(uint8_t val)
{
}