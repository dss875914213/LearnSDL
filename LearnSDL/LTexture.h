#pragma once
#include <string>
#include <iostream>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
using namespace std;

class LTexture
{
public:
	LTexture(); // Initialize variables
	~LTexture(); // Deallocates memory
	bool loadFromFile(string path); // Loads image at specified path
	void free(); //Deallocates texture
	void render(int x, int y); // Renders texture at given point
	int getWidth(); // Gets image dimensions
	int getHeight();

private:
	SDL_Texture* mTexture; // The actual hardware texture
	int mWidth; // Image dimensions
	int mHeight;
};

