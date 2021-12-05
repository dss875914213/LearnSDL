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

//Scene textures
LTexture gPressTexture;
LTexture gUpTexture;
LTexture gDownTexture;
LTexture gLeftTexture;
LTexture gRightTexture;

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
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			cout << "Warning: Linear texture filtering not enabled!" << endl;
		}
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
	
	//Load press texture
	string path = "../Resource/";
	if (!gPressTexture.loadFromFile(path + "press.png"))
	{
		cout << "Failed to load press texture!" << endl;
		success = false;
	}
	//Load up texture
	if (!gUpTexture.loadFromFile(path + "up.png"))
	{
		cout << "Failed to load up texture!" << endl;
		success = false;
	}

	//Load down texture
	if (!gDownTexture.loadFromFile(path + "down.png"))
	{
		cout << "Failed to load down texture!" << endl;
		success = false;
	}

	//Load left texture
	if (!gLeftTexture.loadFromFile(path + "left.png"))
	{
		cout << "Failed to load left texture!" << endl;
		success = false;
	}

	//Load right texture
	if (!gRightTexture.loadFromFile(path + "right.png"))
	{
		cout << "Failed to load right texture!" << endl;
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded image
	gPressTexture.free();
	gUpTexture.free();
	gDownTexture.free();
	gLeftTexture.free();
	gRightTexture.free();

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

			//Current rendered texture
			LTexture* currentTexture = NULL;


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
				}

				//Set texture based on current key state
				const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
				if (currentKeyStates[SDL_SCANCODE_UP])
				{
					currentTexture = &gUpTexture;
				}
				else if (currentKeyStates[SDL_SCANCODE_DOWN])
				{
					currentTexture = &gDownTexture;
				}
				else if (currentKeyStates[SDL_SCANCODE_LEFT])
				{
					currentTexture = &gLeftTexture;
				}
				else if (currentKeyStates[SDL_SCANCODE_RIGHT])
				{
					currentTexture = &gRightTexture;
				}
				else
				{
					currentTexture = &gPressTexture;
				}

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render current texture
				currentTexture->render(0, 0);

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	// Free resources and close SDL
	close();

	return 0;
}


