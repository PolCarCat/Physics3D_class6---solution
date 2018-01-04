#ifndef __j1RENDER_H__
#define __j1RENDER_H__

#include "SDL/include/SDL.h"
#include "Application.h"
#include "p2Point.h"
#include "Module.h"
#include "p2DynArray.h"
#include "Rect.h"
#include "PugiXml\src\pugixml.hpp"

struct RenderPos {

	SDL_Texture* texture;
	int x, y;
	const SDL_Rect* rect = NULL;
	float speed = 1.0f;
	double angle = 0;
	int pivot_x = INT_MAX, pivot_y = INT_MAX;

};
class j1Render : public Module
{
public:

	j1Render(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~j1Render();

	// Called before render is available
	bool Init();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Blit
	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();
	bool Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX, bool scaling = true, bool h_flip = false, bool use_camera = true, SDL_Color color = { 255,255,255 }) const;
	bool BlitGui(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, bool use_camera = false, float speed = 1.0f, double angle = 0, bool h_flip = false, SDL_Color color = {255, 255, 255, 255}, int pivot_x = INT_MAX, int pivot_y = INT_MAX);
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, float speed = 1.0f, bool filled = true, bool use_camera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;

	// Set background color
	void SetBackgroundColor(SDL_Color color);

	// Returns a texture containing the current image of the program
	SDL_Texture* Snapshot();

	
public:

	SDL_Renderer*	renderer;
	fRect			camera;
	SDL_Rect		viewport;
	SDL_Color		background;
	p2DynArray<p2DynArray<RenderPos>> Renderorder;

};

#endif // __j1RENDER_H__