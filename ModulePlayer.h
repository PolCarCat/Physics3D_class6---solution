#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 3000.0f
#define TURN_DEGREES 10.0f * DEGTORAD
#define BRAKE_POWER 1000.0f
#define MAX_SPEED 300

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:

	PhysVehicle3D* vehicle;
	vec3 pos = ( 0,0,0 );
	float turn;
	float acceleration;
	float brake;

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

private:

};

#endif