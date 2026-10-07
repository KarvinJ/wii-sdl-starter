#include "sdl_starter.h"

int startSDLSystems(SDL_Window *window, SDL_Renderer *renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0)
    {
        SDL_LogCritical(1, "SDL crashed. Error: ");
        SDL_Quit();
        return 1;
    }

    if (window == nullptr)
    {
        SDL_LogCritical(1, "Failed to create window: ");
        SDL_Quit();
        return 1;
    }

    if (renderer == nullptr)
    {
        SDL_LogCritical(1, "Failed to create renderer: ");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    if (!IMG_Init(IMG_INIT_PNG))
    {
        SDL_LogCritical(1, "SDL_image crashed. Error: ");
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_LogCritical(1, "SDL_mixer could not initialize!");
        return 1;
    }

    if (TTF_Init() == -1)
    {
        SDL_LogCritical(1, "SDL_ttf could not initialize!");
        return 1;
    }

    return 0;
}

Sprite loadSprite(SDL_Renderer *renderer, const char *filePath, int positionX, int positionY)
{
    SDL_Rect bounds = {positionX, positionY, 0, 0};

    SDL_Texture *texture = IMG_LoadTexture(renderer, filePath);

    if (texture != nullptr)
    {
        SDL_QueryTexture(texture, NULL, NULL, &bounds.w, &bounds.h);
    }

    Sprite sprite = {texture, bounds};

    return sprite;
}

void renderSprite(SDL_Renderer *renderer, Sprite &sprite)
{
    SDL_RenderCopy(renderer, sprite.texture, NULL, &sprite.bounds);
}

Mix_Chunk *loadSound(const char *filePath)
{
    Mix_Chunk *sound = Mix_LoadWAV(filePath);
    if (sound == nullptr)
    {
        SDL_Log("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
    }

    return sound;
}

Mix_Music *loadMusic(const char *filePath)
{
    Mix_Music *music = Mix_LoadMUS(filePath);
    if (music == nullptr)
    {
        SDL_Log("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
    }

    return music;
}

void updateTextureText(SDL_Texture *&texture, const char *text, TTF_Font *&fontSquare, SDL_Renderer *renderer)
{
    SDL_Color fontColor = {255, 255, 255};

    if (fontSquare == nullptr)
    {
        SDL_Log("TTF_OpenFont fontSquare: %s\n", TTF_GetError());
    }

    SDL_Surface *surface = TTF_RenderUTF8_Blended(fontSquare, text, fontColor);
    if (surface == nullptr)
    {
        SDL_Log("TTF_OpenFont: %s\n", TTF_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to create text surface! SDL Error: %s\n", SDL_GetError());
        exit(3);
    }

    SDL_DestroyTexture(texture);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr)
    {
        SDL_Log("TTF_OpenFont: %s\n", TTF_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(surface);
}

// The code for the drawing of the circle is using the Midpoint Circle Algorithm.
void SDL_RenderDrawCircle(SDL_Renderer *renderer, int x, int y, int radius)
{
    int offsetx = 0; 
    int offsety = radius; 
    int diameter = radius - 1;

    while (offsety >= offsetx)
    {
        SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
        SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
        SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
        SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
        SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
        SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
        SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
        SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

        if (diameter >= 2 * offsetx)
        {
            diameter -= 2 * offsetx + 1;
            offsetx += 1;
        }
        else if (diameter < 2 * (radius - offsety))
        {
            diameter += 2 * offsety - 1;
            offsety -= 1;
        }
        else
        {
            diameter += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }
}

void SDL_RenderFillCircle(SDL_Renderer *renderer, int x, int y, int radius)
{
    int offsetx = 0; 
    int offsety = radius; 
    int diameter  = radius - 1;

    while (offsety >= offsetx)
    {
        SDL_RenderDrawLine(renderer, x - offsety, y + offsetx, x + offsety, y + offsetx);
        SDL_RenderDrawLine(renderer, x - offsetx, y + offsety, x + offsetx, y + offsety);
        SDL_RenderDrawLine(renderer, x - offsetx, y - offsety, x + offsetx, y - offsety);
        SDL_RenderDrawLine(renderer, x - offsety, y - offsetx, x + offsety, y - offsetx);

        if (diameter >= 2 * offsetx)
        {
            diameter -= 2 * offsetx + 1;
            offsetx += 1;
        }
        else if (diameter < 2 * (radius - offsety))
        {
            diameter += 2 * offsety - 1;
            offsety -= 1;
        }
        else
        {
            diameter += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }
}

void stopSDLSystems()
{
    Mix_CloseAudio();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void capFrameRate(Uint32 frameStartTime)
{
    Uint32 frameTime = SDL_GetTicks() - frameStartTime;

    if (frameTime < 1000 / FRAME_RATE)
    {
        SDL_Delay(1000 / FRAME_RATE - frameTime);
    }
}