#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int FRAME_RATE = 60;

typedef struct
{
    SDL_Texture *texture;
    SDL_Rect bounds;
} Sprite;

int startSDLSystems(SDL_Window *window, SDL_Renderer *renderer);

Sprite loadSprite(SDL_Renderer *renderer, const char *filePath, int positionX, int positionY);

void renderSprite(SDL_Renderer *renderer, Sprite &sprite);

Mix_Chunk *loadSound(const char *filePath);

Mix_Music *loadMusic(const char *filePath);

void updateTextureText(SDL_Texture *&texture, const char *text, TTF_Font *&fontSquare, SDL_Renderer *renderer);

void SDL_RenderDrawCircle(SDL_Renderer *renderer, int x, int y, int radius);

void SDL_RenderFillCircle(SDL_Renderer *renderer, int x, int y, int radius);

void stopSDLSystems();

void capFrameRate(Uint32 frameStartTime);