#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <iostream>
#include <string>
using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();// Starts up SDL and creates window
bool loadMedia();// Loads media
void close();// Frees media and shuts down SDL
SDL_Texture* loadTexture(string path);//Loads individual images as texture

SDL_Window* gWindow = NULL;// The window we'll be rendering to
SDL_Renderer* gRenderer = NULL;	// The window renderer
SDL_Texture* gTexture = NULL; // Current displayed texture

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
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
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
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
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

	// Load texture
	string path = "../Resource/";
	gTexture = loadTexture(path + "viewport.png");
	if (gTexture == NULL)
	{
		cout << "Failed to load texture image!" << endl;
		success = false;
	}
	//Nothing to load
	return success;
}

void close()
{
	//Free loaded image
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(string path)
{
	SDL_Texture* newTexture = NULL;//The final texture
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		cout << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << endl;
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			cout << "Unable to create texture from " << path << "! SDL Error: " << IMG_GetError() << endl;
		}
		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;
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
				}

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Top left corner viewport
				SDL_Rect topLeftViewport;
				topLeftViewport.x = 0;
				topLeftViewport.y = 0;
				topLeftViewport.w = SCREEN_WIDTH / 2;
				topLeftViewport.h = SCREEN_HEIGHT / 2;
				SDL_RenderSetViewport(gRenderer, &topLeftViewport);

				//Render texture to screen
				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

				//Top right viewport
				SDL_Rect topRightViewport;
				topRightViewport.x = SCREEN_WIDTH / 2;
				topRightViewport.y = 0;
				topRightViewport.w = SCREEN_WIDTH / 2;
				topRightViewport.h = SCREEN_HEIGHT / 2;
				SDL_RenderSetViewport(gRenderer, &topRightViewport);

				//Render texture to screen
				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

				//Bottom viewport
				SDL_Rect bottomViewport;
				bottomViewport.x = 0;
				bottomViewport.y = SCREEN_HEIGHT / 2;
				bottomViewport.w = SCREEN_WIDTH;
				bottomViewport.h = SCREEN_HEIGHT / 2;
				SDL_RenderSetViewport(gRenderer, &bottomViewport);

				//Render texture to screen
				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	// Free resources and close SDL
	close();

	return 0;
}


