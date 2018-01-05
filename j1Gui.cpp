#include "p2Defs.h"
#include "p2Log.h"
#include "p2List.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "j1Fonts.h"
#include "ModuleInput.h"
#include "j1Gui.h"
#include "ModuleWindow.h"
#include "PugiXml\src\pugixml.hpp"
#include "Sprite.h"
#include "Button.h"
#include "Label.h"
#include "InterfaceElement.h"

j1Gui::j1Gui(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Init()
{
	LOG("Loading GUI atlas");
	bool ret = true;

	pugi::xml_node conf;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	App->window->GetWindowSize(gui_size.x, gui_size.y);
	scale = App->window->GetScale();

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	return true;
}

// Update all guis
update_status j1Gui::PreUpdate(float dt)
{

	p2List_item<InterfaceElement*>* current_element = elements.getLast();
	while (current_element != NULL)
	{
		current_element->data->enabled = current_element->data->next_frame_enabled;
		if (current_element->data->isEnabled())
			current_element->data->PreUpdate();
		current_element = current_element->prev;
	}

	/*if (focused_item != nullptr && (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN))
		focused_item->NextElement(focused_item)->setFocus();*/

	return UPDATE_CONTINUE;
}

update_status j1Gui::Update(float dt)
{
	p2List_item<InterfaceElement*>* current_element = elements.getFirst();
	while (current_element != NULL)
	{
		if (current_element->data->isEnabled())
			current_element->data->Update(dt);
		current_element = current_element->next;
	}
	return UPDATE_CONTINUE;
}

// Called after all Updates
update_status j1Gui::PostUpdate(float dt)
{
	p2List_item<InterfaceElement*>* current_element = elements.getFirst();
	while (current_element != NULL)
	{
		if (current_element->data->isEnabled())
			current_element->data->PostUpdate();
		current_element = current_element->next;
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	p2List_item<InterfaceElement*>* current_element = elements.getLast();
	while (current_element != NULL)
	{
		current_element->data->CleanUp();
		delete current_element->data;
		current_element = current_element->prev;
	}
	elements.clear();

	if(atlas_texture != nullptr)
		App->tex->UnLoad(atlas_texture);

	return UPDATE_CONTINUE;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas_texture;
}

uiPoint j1Gui::GetGuiSize()
{
	return gui_size;
}

void j1Gui::setFocus(InterfaceElement * elem)
{
	if (focused_item != nullptr)
		focused_item->in_focus = false;
	focused_item = elem;
	if (focused_item != nullptr)
		focused_item->in_focus = true;
}

InterfaceElement * j1Gui::getFocusedItem()
{
	return focused_item;
}

InterfaceElement * j1Gui::AddElement(InterfaceElement * elem)
{
	if (elements.find(elem) < 0)
		elements.add(elem);

	return elem;
}

bool j1Gui::RemoveElement(InterfaceElement * elem)
{
	int index = elements.find(elem);

	if (index > -1)
		elements.del(elements.atnc(index));
			

	return (index > -1);
}

Sprite* j1Gui::AddSprite(int x, int y, SDL_Texture* tex, bool enabled, SDL_Rect* anim)
{
	Sprite* aux = new Sprite(x, y, tex, enabled, anim);
	elements.add(aux);
	return aux;
}

Label* j1Gui::AddLabel(int x, int y, int psize, const char * font_path, SDL_Color color, Label::RenderMode mode, const char* format, ...)
{
	Label* aux = new Label(x, y, font_path, psize, mode);
	aux->setColor(color);

	if (format != NULL)
	{
		va_list  ap;
		char buffer[TMP_STRING_SIZE];

		va_start(ap, format);
		int res = vsprintf_s(buffer, TMP_STRING_SIZE, format, ap);
		va_end(ap);

		if (res > 0) {
			aux->setString(buffer);
		}
	}

	elements.add(aux);
	return aux;
}

Button* j1Gui::AddButton(int _x, int _y, SDL_Texture* _tex, bool _enabled, SDL_Rect* _anim, Callback callback, SDL_Rect* _hovered_anim, SDL_Rect* _pressed_anim)
{
	Button* aux = new Button(_x, _y, _tex, _enabled, _anim, callback, _hovered_anim, _pressed_anim);

	elements.add(aux);
	return aux;
}

// class Gui ---------------------------------------------------
