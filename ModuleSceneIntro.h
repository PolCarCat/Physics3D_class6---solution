#ifndef __SCENE_INTRO_H__
#define __SCENE_INTRO_H__

#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "PathSegment.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	void AddRoadSegment(bool obstacles = true);

public:
	float prev_base_pos = 0.0f;
	Cube floor, left_ramp, right_ramp;
	PhysBody3D* sensor = nullptr;

	void LoadSegments();
	p2List<SegmentInfo> segments;
	float segment_distance = 0, segment_width = 0;

	Timer countdown;
	float added_time = 0;
};

#endif