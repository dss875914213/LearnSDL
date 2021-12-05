#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "LTexture.h"
#include <iostream>
#include <string>
using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();// Starts up SDL and creates window
bool loadMedia();// Loads media
void close();// Frees media and shuts down SDL

SDL_Window* gWindow = NULL;// The window we'll be rendering to
SDL_Renderer* gRenderer = NULL; // The window renderer
LTexture gArrowTexture;

bool init()
{
	bool success = true; // Initialization flag
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
		success = false;
	}
	else
	{
		// Create window 
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
			success = false;
		}
		else
		{
			//Create renderer for window 
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
					success = false;
				}
			}
		}
	}
	return success;
}

bool loadMedia()
{
	// Loading success flag
	bool success = true;

	// Load front alpha texture
	string path = "../Resource/";
	if (!gArrowTexture.loadFromFile(path + "arrow.png"))
	{
		cout << "Failed to load front texture!" << endl;
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded image
	gArrowTexture.free();

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		cout << "Failed to initialize!" << endl;
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			cout << "Failed to load media!" << endl;
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Angle of rotation
			double degrees = 0;

			//Flip type
			SDL_RendererFlip flipType = SDL_FLIP_NONE;

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					else if (e.type == SDL_KEYDOWN)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_a:
							degrees -= 60;
							break;
						case SDLK_d:
							degrees += 60;
							break;
						case SDLK_q:
							flipType = SDL_FLIP_HORIZONTAL;
							break;
						case SDLK_w:
							flipType = SDL_FLIP_NONE;
							break;
						case SDLK_e:
							flipType = SDL_FLIP_VERTICAL;
							break;
						}
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render arrow
				gArrowTexture.render((SCREEN_WIDTH - gArrowTexture.getWidth()) / 2, (SCREEN_HEIGHT - gArrowTexture.getHeight()) / 2, NULL, degrees, NULL, flipType);
				
				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	// Free resources and close SDL
	close();

	return 0;
}


