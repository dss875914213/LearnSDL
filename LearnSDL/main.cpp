#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include <iostream>
#include <string>
#include "LTexture.h"
using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();// Starts up SDL and creates window
bool loadMedia();// Loads media
void close();// Frees media and shuts down SDL

SDL_Window* gWindow = NULL;// The window we'll be rendering to
SDL_Renderer* gRenderer = NULL; // The window renderer
LTexture gPromptTexture; // Scene texture
//The music that will be played
Mix_Music* gMusic = NULL;

//The sound effects that will be used
Mix_Chunk* gScratch = NULL;
Mix_Chunk* gHigh = NULL;
Mix_Chunk* gMedium = NULL;
Mix_Chunk* gLow = NULL;

bool init()
{
	bool success = true; // Initialization flag
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
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

				//Initialize SDL_mixer
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
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

	//Load prompt texture
	string path = "../Resource/";
	if (!gPromptTexture.loadFromFile(path + "prompt.png"))
	{
		cout << "Failed to load prompt texture!" << endl;
		success = false;
	}

	//Load music
	gMusic = Mix_LoadMUS((path + "beat.wav").c_str());
	if (gMusic == NULL)
	{
		cout << "Failed to load beat music! SDL_mixer Error: " << Mix_GetError() << endl;
		success = false;
	}

	//Load sound effects
	gScratch = Mix_LoadWAV((path + "scratch.wav").c_str());
	if (gScratch == NULL)
	{
		cout << "Failed to load scratch sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		success = false;
	}
	
	gHigh = Mix_LoadWAV((path + "high.wav").c_str());
	if (gHigh == NULL)
	{
		cout << "Failed to load high sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		success = false;
	}

	gMedium = Mix_LoadWAV((path + "medium.wav").c_str());
	if (gMedium == NULL)
	{
		cout << "Failed to load medium sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		success = false;
	}

	gLow = Mix_LoadWAV((path + "low.wav").c_str());
	if (gLow == NULL)
	{
		cout << "Failed to load low sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded image
	gPromptTexture.free();

	//Free the sound effects
	Mix_FreeChunk(gScratch);
	Mix_FreeChunk(gHigh);
	Mix_FreeChunk(gMedium);
	Mix_FreeChunk(gLow);
	gScratch = NULL;
	gHigh = NULL;
	gMedium = NULL;
	gLow = NULL;

	//Free the music 
	Mix_FreeMusic(gMusic);
	gMusic = NULL;

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	Mix_Quit();
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
					else if (e.type == SDL_KEYDOWN)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_1:
							Mix_PlayChannel(-1, gHigh, 0);
							break;

							//Play medium sound effect
						case SDLK_2:
							Mix_PlayChannel(-1, gMedium, 0);
							break;

							//Play low sound effect
						case SDLK_3:
							Mix_PlayChannel(-1, gLow, 0);
							break;

							//Play scratch sound effect
						case SDLK_4:
							Mix_PlayChannel(-1, gScratch, 0);
							break;

						case SDLK_9:
							if (Mix_PlayingMusic() == 0)
							{
								Mix_PlayMusic(gMusic, -1);
							}
							else
							{
								if (Mix_PausedMusic() == 1)
								{
									Mix_ResumeMusic();
								}
								else
								{
									Mix_PauseMusic();
								}
							}
							break;
						case SDLK_0:
							Mix_HaltMusic();
							break;
						}
					}
				}


				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render current texture
				gPromptTexture.render(0, 0);

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	// Free resources and close SDL
	close();

	return 0;
}


