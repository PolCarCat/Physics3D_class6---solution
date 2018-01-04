#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "Module.h"
#include "p2SString.h"
#include "p2Point.h"
#include "SDL\include\SDL_rect.h"

class Sprite;
class Label;
class Button;
class InterfaceElement;

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes

// ---------------------------------------------------
class j1Gui : public Module
{
public:

	j1Gui(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Init();

	// Call before first frame
	bool Start();

	// Called before all Updates
	update_status PreUpdate(float dt);

	// Called every frame
	update_status Update(float dt);

	// Called after all Updates
	update_status PostUpdate(float dt);

	// Called before quitting
	bool CleanUp() override;

	// TODO 2: Create the factory methods
	// Gui creation functions
	Sprite* AddSprite(int x, int y, SDL_Texture* tex, bool enabled = true, SDL_Rect* anim = NULL);
	Label* AddLabel(int x, int y, int psize, const char* font_path = nullptr, SDL_Color color = {0,0,0,255}, Label::RenderMode mode = Label::BLENDED, const char* format = nullptr, ...);
	Button* AddButton(int _x, int _y, SDL_Texture* _tex, bool _enabled, SDL_Rect* _anim, Callback callback, SDL_Rect* _hovered_anim = nullptr, SDL_Rect* _pressed_anim = nullptr);
	const SDL_Texture* GetAtlas() const;
	p2SString atlas_file_name;

	uiPoint GetGuiSize();
	void setFocus(InterfaceElement* elem);
	InterfaceElement* getFocusedItem();

	InterfaceElement* AddElement(InterfaceElement* elem);
	bool RemoveElement(InterfaceElement* elem);

	p2List<InterfaceElement*> elements;

private:
	SDL_Texture* atlas_texture = nullptr;
	uiPoint gui_size;
	float scale;

	InterfaceElement* focused_item = nullptr;
};

#endif // __j1GUI_H__