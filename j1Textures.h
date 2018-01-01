#ifndef __j1TEXTURES_H__
#define __j1TEXTURES_H__

#include "Module.h"
#include "p2List.h"
#include "PugiXml\src\pugixml.hpp"

struct SDL_Texture;
struct SDL_Surface;

class j1Textures : public Module
{
public:

	j1Textures(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~j1Textures();

	// Called before render is available
	bool Init();

	// Called before the first frame
	bool Start();

	// Called before quitting
	bool CleanUp();

	// Load Texture
	SDL_Texture* const	Load(const char* path);
	bool				UnLoad(SDL_Texture* texture);
	SDL_Texture* const	LoadSurface(SDL_Surface* surface);
	void				GetSize(const SDL_Texture* texture, uint& width, uint& height) const;

public:

	p2List<SDL_Texture*>	textures;
};


#endif // __j1TEXTURES_H__