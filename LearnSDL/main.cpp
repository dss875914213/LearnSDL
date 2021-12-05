#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "LTexture.h"
#include <iostream>
#include <string>
#include "LButton.h"
using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


bool init();// Starts up SDL and creates window
bool loadMedia();// Loads media
void close();// Frees media and shuts down SDL

SDL_Window* gWindow = NULL;// The window we'll be rendering to
SDL_Renderer* gRenderer = NULL; // The window renderer

//Mouse button sprites
SDL_Rect gSpriteClips[BUTTON_SPRITE_MOUSE_TOTAL];
LTexture gButtonSpriteSheetTexture; // Rendered texture

//Buttons objects
LButton gButtons[TOTAL_BUTTONS];

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

	// Open the font
	string path = "../Resource/";
	if (!gButtonSpriteSheetTexture.loadFromFile(path+"button.png"))
	{
		cout << "Failed to load button sprite texture! " << endl;
		success = false;
	}
	else
	{
		//Set sprites
		for (int i = 0; i < BUTTON_SPRITE_MOUSE_TOTAL; i++)
		{
			gSpriteClips[i].x = 0;
			gSpriteClips[i].y = i * 200;
			gSpriteClips[i].w = BUTTON_WIDTH;
			gSpriteClips[i].h = BUTTON_HEIGHT;
		}

		//Set buttons in corners
		gButtons[0].setPosition(0, 0);
		gButtons[1].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, 0);
		gButtons[2].setPosition(0, SCREEN_HEIGHT - BUTTON_HEIGHT);
		gButtons[3].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);
	}

	return success;
}

void close()
{
	//Free loaded image
	gButtonSpriteSheetTexture.free();

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

					//Handle button events
					for (int i = 0; i < TOTAL_BUTTONS; i++)
					{
						gButtons[i].handleEvent(&e);
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render buttons
				for (int i = 0; i < TOTAL_BUTTONS; i++)
				{
					gButtons[i].render();
				}

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	// Free resources and close SDL
	close();

	return 0;
}


