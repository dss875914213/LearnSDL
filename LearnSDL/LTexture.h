#pragma once
#include <string>
#include <iostream>
#include "SDL/SDL_ttf.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
using namespace std;

class LTexture
{
public:
	LTexture(); // Initialize variables
	~LTexture(); // Deallocates memory
	bool loadFromFile(string path); // Loads image at specified path
#ifdef SDL_TTF_MAJOR_VERSION 
	bool loadFromRenderedText(string textureText, SDL_Color textColor);// Creates image from font string
#endif
	void free(); //Deallocates texture
	void setColor(Uint8 red, Uint8 green, Uint8 blue);//Set color modulation
	void setBlendMode(SDL_BlendMode blending); // Set blending
	void setAlpha(Uint8 alpha); // Set alpha modulation
	void render(int x, int y, SDL_Rect* clip=NULL, double angle=0.0,SDL_Point* center=NULL, SDL_RendererFlip flip=SDL_FLIP_NONE); // Renders texture at given point
	int getWidth(); // Gets image dimensions
	int getHeight();

private:
	SDL_Texture* mTexture; // The actual hardware texture
	int mWidth; // Image dimensions
	int mHeight;
};

