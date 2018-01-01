#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	void GetWindowSize(uint& width, uint& height) const  {width = SCREEN_WIDTH * SCREEN_SIZE; height = SCREEN_HEIGHT * SCREEN_SIZE; }

	float GetScale() const { return SCREEN_SIZE; }
};

#endif // __ModuleWindow_H__