#include <wiiuse/wpad.h>
#include "sdl_starter.h"

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

bool isRunning = true;
bool isPaused = false;

const int PLAYER_SPEED = 600;

SDL_Rect player = {SCREEN_WIDTH / 2 - 64, SCREEN_HEIGHT / 2 - 64, 64, 64};

SDL_Rect ball = {SCREEN_WIDTH / 2 + 50, SCREEN_HEIGHT / 2, 32, 32};

int ballVelocityX = 400;
int ballVelocityY = 400;

void handleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			isRunning = false;
		}
	}
}

void update(float deltaTime)
{
	// WPAD_ButtonsHeld tells us which buttons are keep pressing in this loop
	const u32 padHeld = WPAD_ButtonsHeld(0);

	if (padHeld & WPAD_BUTTON_LEFT && player.x > 0)
	{
		player.x -= PLAYER_SPEED * deltaTime;
	}

	else if (padHeld & WPAD_BUTTON_RIGHT && player.x < SCREEN_WIDTH - player.w)
	{
		player.x += PLAYER_SPEED * deltaTime;
	}

	else if (padHeld & WPAD_BUTTON_UP && player.y > 0)
	{
		player.y -= PLAYER_SPEED * deltaTime;
	}

	else if (padHeld & WPAD_BUTTON_DOWN && player.y < SCREEN_HEIGHT - player.h)
	{
		player.y += PLAYER_SPEED * deltaTime;
	}

	if (ball.x < 0 || ball.x > SCREEN_WIDTH - ball.w)
	{
		ballVelocityX *= -1;
	}

	else if (ball.y < 0 || ball.y > SCREEN_HEIGHT - ball.h)
	{
		ballVelocityY *= -1;
	}

	else if (SDL_HasIntersection(&player, &ball))
	{
		ballVelocityX *= -1;
		ballVelocityY *= -1;
	}

	ball.x += ballVelocityX * deltaTime;
	ball.y += ballVelocityY * deltaTime;
}

void render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &player);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &ball);

	SDL_RenderPresent(renderer);
}

int main(int argc, char **argv)
{
	window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (startSDLSystems(window, renderer) > 0)
	{
		return 1;
	}

	Uint32 previousFrameTime = SDL_GetTicks();
	Uint32 currentFrameTime = previousFrameTime;
	float deltaTime = 0.0f;

	while (true)
	{
		currentFrameTime = SDL_GetTicks();
		deltaTime = (currentFrameTime - previousFrameTime) / 1000.0f;
		previousFrameTime = currentFrameTime;

		// PAD_ButtonsDown tells us which buttons were pressed in this loop
		// this is a "one shot" state which will not fire again until the button has been released
		const u32 padDown = WPAD_ButtonsDown(0);

		// if (padDown & WPAD_BUTTON_SELECT)
		// {
		// 	isRunning = false;
		// }

		// We Pause the game when the Start button is pressed, and unpause it when pressed again
		if (padDown & WPAD_BUTTON_HOME)
		{
			isPaused = !isPaused;
		}

		handleEvents();

		if (!isPaused)
		{
			update(deltaTime);
		}

		render();
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	exit(0);
}