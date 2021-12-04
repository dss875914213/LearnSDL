#include "SDL/SDL.h"
#include <iostream>
#include <string>
using namespace std;

// Key press surface constants
enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();// Starts up SDL and creates window
bool loadMedia();// Loads media
void close();// Frees media and shuts down SDL
SDL_Surface* loadSurface(string path);//Loads individual image

SDL_Window* gWindow = NULL;// The window we'll be rendering to
SDL_Surface* gScreenSurface = NULL; // The surface contained by the window
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];//The images that correspond to a key press
SDL_Surface* gCurrentSurface = NULL;//Current displayed image
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

SDL_Surface* loadSurface(string path)
{
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (loadedSurface == NULL)
	{
		cout << "Unable to load image " << path << "! SDL ERROR:" << SDL_GetError() << endl;
	}
	return loadedSurface;
}

bool loadMedia()
{
	// Loading success flag
	bool success = true;
	string path = "../Resource/";
	//Load default surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface(path + "press.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
	{
		cout << "Failed to load default image!" << endl;
		success = false;
	}

	//Load up surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface(path + "up.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL)
	{
		cout << "Failed to load up image!" << endl;
		success = false;
	}

	//Load down surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface(path + "down.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL)
	{
		cout << "Failed to load down image!" << endl;
		success = false;
	}

	//Load left surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface(path + "left.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL)
	{
		cout << "Failed to load left image!" << endl;
		success = false;
	}

	//Load right surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface(path + "right.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL)
	{
		cout << "Failed to load right image!" << endl;
		success = false;
	}
	return success;
}

void close()
{
	//Deallocate surface
	for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; i++)
	{
		SDL_FreeSurface(gKeyPressSurfaces[i]);
		gKeyPressSurfaces[i] = NULL;
	}
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
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Set default current surface
			gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
			
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
					else if (e.type == SDL_KEYDOWN) // User presses a key
					{
						//Select surfaces based on key press
						switch (e.key.keysym.sym)
						{
						case SDLK_UP:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
							break;
						case SDLK_DOWN:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
							break;
						case SDLK_LEFT:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
							break;
						case SDLK_RIGHT:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
							break;
						default:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
							break;
						}
					}
				}
				// Apply the image
				SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
				//Update the surface
				SDL_UpdateWindowSurface(gWindow);
			}
		}
	}
	// Free resources and close SDL
	close();

	return 0;
}


