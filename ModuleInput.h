#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"
#include "Globals.h"

#define MAX_MOUSE_BUTTONS 5

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

typedef struct {
	KEY_STATE keyState = KEY_IDLE;
	bool blocked = false;
} keyEvent;

class ModuleInput : public Module
{
public:
	
	ModuleInput(Application* app, bool start_enabled = true);
	~ModuleInput();

	bool Init();
	update_status PreUpdate(float dt);
	bool CleanUp();

	KEY_STATE GetKey(int id) const
	{
		return keyboard[id].keyState;
	}

	KEY_STATE GetMouseButton(int id) const
	{
		return mouse_buttons[id].keyState;
	}

	KEY_STATE GetMouseButtonDown(int id) const
	{
		if (mouse_buttons[id - 1].blocked)
			return KEY_IDLE;
		return mouse_buttons[id - 1].keyState;
	}


	int GetMouseX() const
	{
		return mouse_x;
	}

	int GetMouseY() const
	{
		return mouse_y;
	}

	int GetMouseZ() const
	{
		return mouse_z;
	}

	int GetMouseXMotion() const
	{
		return mouse_x_motion;
	}

	int GetMouseYMotion() const
	{
		return mouse_y_motion;
	}

	void GetMousePosition(int& x, int& y)
	{
		x = mouse_x;
		y = mouse_y;
	}


	bool BlockMouseEvent(int event_id) {
		if (mouse_buttons[event_id - 1].blocked)
			return false;
		mouse_buttons[event_id - 1].blocked = true;
		return true;
	}

	bool BlockKeyboardEvent(int event_id) {
		if (keyboard[event_id].blocked)
			return false;
		keyboard[event_id].blocked = true;
		return true;
	}

	void BlockMouse() {
		mouse_x = mouse_y = INT_MAX - 2;
	}

private:
	keyEvent* keyboard;
	keyEvent mouse_buttons[MAX_MOUSE_BUTTONS];
	int mouse_x;
	int mouse_y;
	int mouse_z;
	int mouse_x_motion;
	int mouse_y_motion;
	//int mouse_z_motion;
};

#endif