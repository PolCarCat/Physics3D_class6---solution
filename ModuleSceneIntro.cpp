#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "j1Gui.h"
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

	countdown.Start();
	added_time = 0;

	LoadSegments();
	LoadRecord();
	//App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(App->player->pos);

	//s.size = vec3(5, 3, 1);
	//s.SetPos(0, 2.5f, 20);

	//sensor = App->physics->AddBody(s, 0.0f);
	//sensor->SetAsSensor(true);
	//sensor->collision_listeners.add(this);

	/*Cube random_c;
	random_c.size = { 20 ,40 ,5 };
	random_c.SetPos(0, 0, 50);
	random_c.SetRotation(75, { 1,0,0 });	
	PhysBody3D* random = App->physics->AddBody(random_c, 0);*/
	
	left_ramp.size = { 20, 50, 1800 };
	left_ramp.SetPos(60, -6, prev_base_pos);
	left_ramp.SetRotation(280, { 0,0,1 });

	right_ramp.size = { 20, 50, 1800 };
	right_ramp.SetPos(-60, -6, prev_base_pos);
	right_ramp.SetRotation(80, { 0,0,1 });

	floor.size = { 100, 0, 1800 };
	floor.color = White;
	floor.SetPos(0, 0, prev_base_pos);

	prev_base_pos = 0;

	AddRoadSegment(false);
	AddRoadSegment();

	segments_completed = 0;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	SetRecord();
	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	float camera_speed = 5.0f;
	Plane p(0, 1, 0, 0);



	curr_time = (20 - countdown.ReadSec() + added_time);


	if (App->player->pos.y < -30 || App->input->GetKey(SDL_SCANCODE_R) == KEY_STATE::KEY_DOWN || curr_time <= 0)
	{
		App->Restart();
		App->player->in_intro = true;
		
	}

	vec3 diff_pos;
	diff_pos.p -= 20;
	vec3 camera_pos = App->player->pos;
	float player_y_rot, player_x_rot, player_z_rot;
	App->player->vehicle->GetRotation(player_x_rot, player_y_rot, player_z_rot);
	diff_pos = rotate(diff_pos, player_x_rot * 180 / M_PI, {0, 1, 0});

	camera_pos += diff_pos;
	

	/*left_ramp.Render();
	right_ramp.Render();
	floor.Render();*/


	if (segments_completed > record)
	{
		record = segments_completed;
	}

	p.axis = true;
	p.wire = true;
	//p.Render();
	App->camera->Move(normalize(camera_pos - App->camera->Position) * dt * camera_speed * length(camera_pos - App->camera->Position));
	App->camera->Position.y = 10.0f;
	App->camera->LookAt(App->player->pos);
	//sensor->GetTransform(&s.transform);
	//s.Render();




	char title[80];
	sprintf_s(title, "%.1f Km/h, %.1f sec", App->player->vehicle->GetKmh(), curr_time);
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	LOG("Sensor hit!");
	if (body1->collision_listeners.getFirst()->data == this) {
		body1->Destroy();
		body1->SetAsSensor(false);
		added_time += 1;
		segments_completed++;
	}
	else {
		body2->Destroy();
		body2->SetAsSensor(false);
		added_time += 1;
		segments_completed++;
	}
	
	AddRoadSegment();
}

void ModuleSceneIntro::AddRoadSegment(bool obstacles)
{
	int segment_index = rand() % segments.count();

	SegmentInfo info;
	info = segments.at(segment_index)->data;

	Cube base, left, right;
	base.color = White;
	base.size = { segment_width, 0, segment_distance };
	base.SetPos(0, 0, prev_base_pos);
	App->physics->AddBody(base, 0.0f);

	left.size = { 20, 50, segment_distance };
	left.SetPos(segment_width / 2, -6, prev_base_pos);
	left.SetRotation(300, { 0,0,1 });
	App->physics->AddBody(left, 0.0f);

	right.size = { 20, 50, segment_distance };
	right.SetPos(-segment_width / 2, -6, prev_base_pos);
	right.SetRotation(60, { 0,0,1 });
	App->physics->AddBody(right, 0.0f);

	Cube s;
	s.size = vec3(segment_width + 80, 120, 1);
	s.SetPos(0, 60, prev_base_pos + segment_distance / 2);
	s.color.Set(1.0f, 1.0f, 1.0f, 2.0f);
	sensor = App->physics->AddBody(s, 0.0f);
	sensor->SetAsSensor(true);
	sensor->collision_listeners.add(this);
	sensor->SetVisible(false);
	
	if (obstacles) {
		App->physics->AddSensor({ (float)(rand() % 160 - 80), 3.0f, (float)(rand() % (int)segment_distance - (segment_distance / 2)) + prev_base_pos }, (rand()%2));

		for (uint i = 0; i < info.num_obstacles; i++) {
			Cube c;
			Sphere s;
			Cylinder cy;
			ObstacleInfo o_info = info.obstacles[i];
			switch (o_info.type) {
			case WALL:
				c.color = Red;
				c.size = o_info.dims;
				c.SetPos(o_info.pos.x, o_info.pos.y, o_info.pos.z + prev_base_pos);
				c.SetRotation(o_info.rotation.x, { 1,0,0 });
				c.SetRotation(o_info.rotation.y, { 0,1,0 });
				c.SetRotation(o_info.rotation.z, { 0,0,1 });
				App->physics->AddBody(c, 0.0f);
				break;
			case BOULDER:
				s.color = Red;
				//s.size = o_info.dims;
				s.SetPos(o_info.pos.x, o_info.pos.y, o_info.pos.z + prev_base_pos);
				s.SetRotation(o_info.rotation.x, { 1,0,0 });
				s.SetRotation(o_info.rotation.y, { 0,1,0 });
				s.SetRotation(o_info.rotation.z, { 0,0,1 });
				s.radius = o_info.radius;
				App->physics->AddBody(s, 10000.0f);
				break;
			case CYLINDER:
				cy.color = Red;
				cy.height = o_info.dims.y;
				cy.radius = o_info.radius;
				cy.SetPos(o_info.pos.x, o_info.pos.y, o_info.pos.z + prev_base_pos);
				cy.SetRotation(o_info.rotation.x, { 1,0,0 });
				cy.SetRotation(o_info.rotation.y, { 0,1,0 });
				cy.SetRotation(o_info.rotation.z, { 0,0,1 });
				App->physics->AddBody(cy, 10000.0f);
				break;
			}
		}
	}

	floor.SetPos(0, 0, prev_base_pos - segment_distance);
	left_ramp.SetPos(60, -6, prev_base_pos - segment_distance);
	right_ramp.SetPos(-60, -6, prev_base_pos - segment_distance);

	prev_base_pos += segment_distance;
}


void ModuleSceneIntro::LoadRecord()
{
	pugi::xml_document segment_doc;
	pugi::xml_parse_result result = segment_doc.load_file("Segments.xml");

	record = segment_doc.child("Record").attribute("record").as_int();
}

void ModuleSceneIntro::SetRecord()
{
	pugi::xml_document segment_doc;
	pugi::xml_parse_result result = segment_doc.load_file("Segments.xml");

	segment_doc.append_child("Record").append_attribute("record") = record;

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
		segment_distance = segments_list_node.attribute("Length").as_float(100.0f);
		segment_width = segments_list_node.attribute("Width").as_float(80.0f);
		int j = 0;
		for (pugi::xml_node segment_node : segments_list_node.children()) {
			LOG("Loading segment %d", ++j);
			SegmentInfo info;
			info.rotation.x = segment_node.attribute("RotationX").as_float(0.0f);
			info.rotation.y = segment_node.attribute("RotationY").as_float(0.0f);
			info.rotation.z = segment_node.attribute("RotationZ").as_float(0.0f);

			pugi::xml_node obstacle_info_list_node = segment_node.child("Obstacles");
			info.num_obstacles = obstacle_info_list_node.attribute("Number").as_uint(0);
			if (info.num_obstacles > 0) {
				info.obstacles = (ObstacleInfo*)malloc(info.num_obstacles * sizeof ObstacleInfo);
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
					o_info.type = (ObstacleType)obstacle_node.attribute("Type").as_int(0);
					info.obstacles[i] = o_info;
					LOG("	Loading obstacle %d", ++i);
				}
			}
			segments.add(info);
		}
	}
}

