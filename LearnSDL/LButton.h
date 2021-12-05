#pragma once
#include "SDL/SDL.h"

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_MOUSE_TOTAL = 4
};

//Button constants
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

class LButton
{
public:
	LButton(); //Initialize internal variables
	void setPosition(int x, int y); // Sets top left position
	void handleEvent(SDL_Event* e); // Handles mouse event
	void render(); // Shows button sprite

private:
	SDL_Point mPosition; //Top left position
	LButtonSprite mCurrentSprite;
};

