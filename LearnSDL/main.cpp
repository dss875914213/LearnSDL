#include "SDL/SDL.h"
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
SDL_Surface* gScreenSurface = NULL; // The surface contained by the window
SDL_Surface* gHelloWorld = NULL;// The image we will load and show on the screen

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
		gWindow = SDL_CreateWindow("SDL Tutorial1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}
	return success;
}

bool loadMedia()
{
	// Loading success flag
	bool success = true;
	//Load splash image ¼ÓÔØÆô¶¯»­Ãæ
	string path = "../Resource/Hello.bmp";
	gHelloWorld = SDL_LoadBMP(path.c_str());
	if (gHelloWorld == NULL)
	{
		cout << "Unable to load image " << path << "! SDL Error: " << SDL_GetError() << endl;
		success = false;
	}
	return success;
}

void close()
{
	//Deallocate surface
	SDL_FreeSurface(gHelloWorld);
	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	//Quit SDL subsystems
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
			// Apply the image
			SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
			//Update the surface
			SDL_UpdateWindowSurface(gWindow);
			//Wait two seconds
			SDL_Delay(2000);
		}
	}
	// Free resources and close SDL
	close();

	return 0;
}


