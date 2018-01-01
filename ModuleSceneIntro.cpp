#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"
#include "PugiXml\src\pugixml.hpp"
#include "Time.h"
#include <Random>


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	srand(time(nullptr));

	LoadSegments();
	
	//App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(App->player->pos);

	//s.size = vec3(5, 3, 1);
	//s.SetPos(0, 2.5f, 20);

	//sensor = App->physics->AddBody(s, 0.0f);
	//sensor->SetAsSensor(true);
	//sensor->collision_listeners.add(this);

	Cube random_c;
	random_c.size = { 20 ,40 ,5 };
	random_c.SetPos(0, 0, 50);
	random_c.SetRotation(75, { 1,0,0 });

	left_ramp.size = { 20, 50, 1200 };
	left_ramp.SetPos(60, -6, 0);
	left_ramp.SetRotation(280, { 0,0,1 });
	App->physics->AddBody(left_ramp, 0);

	right_ramp.size = { 20,50,1200 };
	right_ramp.SetPos(-60, -6, 0);
	right_ramp.SetRotation(80, { 0,0,1 });
	App->physics->AddBody(right_ramp, 0);

	floor.size = { 100, 0, 1200 };
	floor.color.Set(3.0f, 3.0f, 3.5f);
	floor.SetPos(0, 0, 0);


	PhysBody3D* random = App->physics->AddBody(random_c, 0);


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	float camera_speed = 10.0f;
	Plane p(0, 1, 0, 0);

	//Meh
	if (App->player->pos.z >= 200)
	{
		vec3 camera_diff = App->camera->Position - App->player->pos;
		App->player->vehicle->SetPos(App->player->pos.s, App->player->pos.t, -200);
		btVector3 btpos = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin();
		App->player->pos = { btpos.getX(), btpos.getY(), btpos.getZ() };
		App->camera->Position = App->player->pos + camera_diff;
	}

	vec3 camera_pos = App->player->pos;
	camera_pos.p -= 20;

	left_ramp.Render();
	right_ramp.Render();
	floor.Render();

	p.axis = true;
	p.wire = true;
	//p.Render();
	App->camera->Move(normalize(camera_pos - App->camera->Position) * dt * camera_speed * length(camera_pos - App->camera->Position));
	App->camera->Position.y = 10.0f;
	App->camera->LookAt(App->player->pos);
	//sensor->GetTransform(&s.transform);
	//s.Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	LOG("Hit!");
}

void ModuleSceneIntro::AddRoadSegment()
{
	int segment_index = rand() % segments.count();

	Cube base;
	SegmentInfo info;
	segments.at(segment_index);
	base.color = White;
	base.size = { 40, 20, 5 };
	App->physics->AddBody(base, 0.0f);

	prev_base_pos += 600;
	floor.SetPos(0, 0, prev_base_pos);

	left_ramp.SetPos(60, -6, prev_base_pos);
	//left_ramp.SetRotation(280, { 0,0,1 });
	App->physics->AddBody(left_ramp, 0);

	right_ramp.SetPos(-60, -6, prev_base_pos);
	//right_ramp.SetRotation(80, { 0,0,1 });
	App->physics->AddBody(right_ramp, 0);
}

void ModuleSceneIntro::LoadSegments()
{
	pugi::xml_document segment_doc;
	pugi::xml_parse_result result = segment_doc.load_file("Segments.xml");

	if (result == NULL) {
		LOG("Could not load map xml file Segments.xml. pugi error: %s", result.description());
	}
	else {
		pugi::xml_node segments_list_node = segment_doc.child("Segments");

		for (pugi::xml_node segment_node : segments_list_node.children()) {
			SegmentInfo info;
			info.rotation.x = segment_node.attribute("RotationX").as_float(0.0f);
			info.rotation.y = segment_node.attribute("RotationY").as_float(0.0f);
			info.rotation.z = segment_node.attribute("RotationZ").as_float(0.0f);

			pugi::xml_node obstacle_info_list_node = segment_node.child("Obstacles");
			uint obstacle_count = obstacle_info_list_node.attribute("Number").as_uint(0);
			info.obstacles = new ObstacleInfo[obstacle_count];
			uint i = 0;
			for (pugi::xml_node obstacle_node : obstacle_info_list_node.children()) {
				ObstacleInfo o_info;
				o_info.dims.x = obstacle_node.attribute("Width").as_float(0.0f);
				o_info.dims.y = obstacle_node.attribute("Height").as_float(0.0f);
				o_info.dims.z = obstacle_node.attribute("Depth").as_float(0.0f);
				o_info.dynamic = obstacle_node.attribute("Dynamic").as_bool(false);
				o_info.pos.x = obstacle_node.attribute("PosX").as_float(0.0f);
				o_info.pos.y = obstacle_node.attribute("PosY").as_float(0.0f);
				o_info.pos.z = obstacle_node.attribute("PosZ").as_float(0.0f);
				o_info.radius = obstacle_node.attribute("Radius").as_float(0.0f);
				o_info.rotation.x = obstacle_node.attribute("RotationX").as_float(0.0f);
				o_info.rotation.y = obstacle_node.attribute("RotationY").as_float(0.0f);
				o_info.rotation.z = obstacle_node.attribute("RotationZ").as_float(0.0f);
				switch (obstacle_node.attribute("Type").as_int(0)) {
				case WALL:
					o_info.type = WALL;
					break;
				case BOULDER:
					o_info.type = BOULDER;
					break;
				default:
					o_info.type = WALL;
					break;
				}
				info.obstacles[i] = o_info;
			}
			segments.add(info);
		}
	}
}

